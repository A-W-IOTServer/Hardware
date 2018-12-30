
// Load Wi-Fi library
#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>
// Replace with your network credentials
const char* ssid     = "Straubing 2.4";
const char* password = "twodoubleyous";

SoftwareSerial Mega(4, 5);
// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
String header;

// Assign output variables to GPIO pins
// I will these 4 pins as a binary with GPIOPin 4 being the MSB
//const int GPIOPin1 = 4;
//const int GPIOPin2 = 5;
const int GPIOPin1 = 100;
const int GPIOPin2 = 101;
const int GPIOPin3 = 9;
const int GPIOPin4 = 10;

//I will use this pin to trigger an interrupt on an arduino!


void setup() {
  Serial.begin(9600);
  Mega.begin(9600);
  // Initialize the output variables as outputs
  pinMode(GPIOPin1, OUTPUT);
  pinMode(GPIOPin2, OUTPUT);
  pinMode(GPIOPin3, OUTPUT);
  pinMode(GPIOPin4, OUTPUT);
  //pinMode(triggerPin, OUTPUT);

  // Set outputs to LOW
  digitalWrite(GPIOPin1, LOW);
  digitalWrite(GPIOPin2, LOW);
  digitalWrite(GPIOPin3, LOW);
  digitalWrite(GPIOPin4, LOW);
  //  digitalWrite(triggerPin, LOW);

  // Connect to Wi-Fi network with SSID and password
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

void loop() {


  WiFiClient client = server.available();   // Listen for incoming clients

  if (client) {                             // If a new client connects,
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
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

            // turns the GPIOs on and off
            if (header.indexOf("GET /0/") >= 0) {

              digitalWrite(GPIOPin1, LOW);
              digitalWrite(GPIOPin2, LOW);
              digitalWrite(GPIOPin3, LOW);
              digitalWrite(GPIOPin4, LOW);

            }
            else if (header.indexOf("GET /1/") >= 0) {

              digitalWrite(GPIOPin1, HIGH);
              digitalWrite(GPIOPin2, LOW);
              digitalWrite(GPIOPin3, LOW);
              digitalWrite(GPIOPin4, LOW);

            }

            else if (header.indexOf("GET /2/") >= 0) {

              digitalWrite(GPIOPin1, LOW);
              digitalWrite(GPIOPin2, HIGH);
              digitalWrite(GPIOPin3, LOW);
              digitalWrite(GPIOPin4, LOW);
            }
            else if (header.indexOf("GET /3/") >= 0) {

              digitalWrite(GPIOPin1, HIGH);
              digitalWrite(GPIOPin2, HIGH);
              digitalWrite(GPIOPin3, LOW);
              digitalWrite(GPIOPin4, LOW);
            }
            else if (header.indexOf("GET /4/") >= 0) {

              digitalWrite(GPIOPin1, LOW);
              digitalWrite(GPIOPin2, LOW);
              digitalWrite(GPIOPin3, HIGH);
              digitalWrite(GPIOPin4, LOW);
            }
            else if (header.indexOf("GET /5/") >= 0) {

              digitalWrite(GPIOPin1, HIGH);
              digitalWrite(GPIOPin2, LOW);
              digitalWrite(GPIOPin3, HIGH);
              digitalWrite(GPIOPin4, LOW);
            }
            else if (header.indexOf("GET /6/") >= 0) {

              digitalWrite(GPIOPin1, LOW);
              digitalWrite(GPIOPin2, HIGH);
              digitalWrite(GPIOPin3, HIGH);
              digitalWrite(GPIOPin4, LOW);
            }
            else if (header.indexOf("GET /7/") >= 0) {

              digitalWrite(GPIOPin1, HIGH);
              digitalWrite(GPIOPin2, HIGH);
              digitalWrite(GPIOPin3, HIGH);
              digitalWrite(GPIOPin4, LOW);
            }
            else if (header.indexOf("GET /8/") >= 0) {

              digitalWrite(GPIOPin1, LOW);
              digitalWrite(GPIOPin2, LOW);
              digitalWrite(GPIOPin3, LOW);
              digitalWrite(GPIOPin4, HIGH);
            }
            else if (header.indexOf("GET /9/") >= 0) {

              digitalWrite(GPIOPin1, HIGH);
              digitalWrite(GPIOPin2, LOW);
              digitalWrite(GPIOPin3, LOW);
              digitalWrite(GPIOPin4, HIGH);
            }
            else if (header.indexOf("GET /A/") >= 0) {

              digitalWrite(GPIOPin1, LOW);
              digitalWrite(GPIOPin2, HIGH);
              digitalWrite(GPIOPin3, LOW);
              digitalWrite(GPIOPin4, HIGH);
            }
            else if (header.indexOf("GET /B/") >= 0) {

              digitalWrite(GPIOPin1, HIGH);
              digitalWrite(GPIOPin2, HIGH);
              digitalWrite(GPIOPin3, LOW);
              digitalWrite(GPIOPin4, HIGH);
            }
            else if (header.indexOf("GET /C/") >= 0) {

              digitalWrite(GPIOPin1, LOW);
              digitalWrite(GPIOPin2, LOW);
              digitalWrite(GPIOPin3, HIGH);
              digitalWrite(GPIOPin4, HIGH);
            }
            else if (header.indexOf("GET /D/") >= 0) {

              digitalWrite(GPIOPin1, HIGH);
              digitalWrite(GPIOPin2, LOW);
              digitalWrite(GPIOPin3, HIGH);
              digitalWrite(GPIOPin4, HIGH);
            }
            else if (header.indexOf("GET /E/") >= 0) {

              digitalWrite(GPIOPin1, LOW);
              digitalWrite(GPIOPin2, HIGH);
              digitalWrite(GPIOPin3, HIGH);
              digitalWrite(GPIOPin4, HIGH);
            }
            else if (header.indexOf("GET /F/") >= 0) {

              digitalWrite(GPIOPin1, HIGH);
              digitalWrite(GPIOPin2, HIGH);
              digitalWrite(GPIOPin3, HIGH);
              digitalWrite(GPIOPin4, HIGH);
            }
            if (Mega.available()) {
              Serial.println(Mega.read());
            }
            Mega.write('L'); //L stands for Lights
            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
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
