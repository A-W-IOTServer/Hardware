
// Load Wi-Fi library
#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>
#include <ESP8266HTTPClient.h>


//====== Replace with your network credentials =========

const char* ssid     = "I am better than Kyle";
const char* password = "analbeads";
const char* serverEndpoint = "http://74.140.205.4:80/test.php";

//=======Server Handlers=========

//-----Server Controller------
HTTPClient http;    //Declare object of class HTTPClient

//-----ESP Server Controller----
WiFiServer server(80);

// Variable to store the HTTP request
String header;

//=======Mega serial communication======

SoftwareSerial Mega(12, 14);

//--------Handles Serial Communication as well as Pi server comms----------
String response = "";
volatile bool firstTerminator = false;
volatile bool readyToPrint = false;




void setup()
{

  pinMode(4,OUTPUT);
  digitalWrite(4,HIGH);
  //====Serial Setup=====

  Serial.begin(9600);
  Mega.begin(9600);

  //=========ESP Sever Setup===========

  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();

}

void loop()
{

  WiFiClient client = server.available();   // Listen for incoming client
  http.begin(serverEndpoint);      //Specify request destination

  //------------Handle reading data back from the Arduino/Sending data to the server-----------------
  if (readyToPrint) {
    Serial.println("We are about to send to the server");
    //Set up the Arduino response to send to the server.
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");  //Specify content-type header
    int httpData = http.POST("post=" + response + "\n");  //Send the request
    http.end();  //Close connection

    //Uncomment Below to see data to and from the server.
    /*
      Serial.println(response);
      String payload = http.getString();
      Serial.println(httpData);   //Print HTTP return code
      Serial.println(payload);    //Print request response payload
    */

    //Reset the Terminator flags/ response
    firstTerminator = false;
    readyToPrint = false;
    response = "";
  }
  else {
    //If the Arduino sends Serial data then read
    while (Mega.available()) {
      char tempChar = Mega.read();

      //Below handles if the ending sequence
      if (tempChar == '$') {
        firstTerminator = true;
        continue;
      }
      if (firstTerminator) {
        if (tempChar == 'k') {
          readyToPrint = true;
          break;
        }
        else {
          firstTerminator = false;
        }
      }
      response += tempChar;
    }
  }

  //-----------------Handle Client ESP Communication------------------
  if (client) {                             // If a new client connects,
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected())
    { // loop while the client's connected


      if (client.available())
      { // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();

            //======================Server Endpoints=========================

            if (header.indexOf("GET /toggleLights/") >= 0)
            {
              Serial.println("Here");
              Mega.write("toggleLights"); //send the command toggleLights
            }


            //======================END Server Endpoints====================
            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          }
          else
          { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        }
        else if (c != '\r')
        { // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}
