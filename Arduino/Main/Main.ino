//=======================Imports=======================//

#include <SoftwareSerial.h>
#include <String.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

//=======================Pin Setup=======================//

const int lightPin = 2;

//=======================Globals=======================//

//---Integers---

//---Strings---
volatile String command = "";
volatile String sendData = "";
//---Booleans---
bool lightOn = false;

//=====Serial for Ardiuno Nano========
SoftwareSerial esp(7, 8);

//=========LCD===============//

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


void setup()
{
  //========Timer Setup========//

  //Below I set up timer1/counter1 to trigger TIMER1_OVF_vect every 4.19 seconds.
  //This interrupt will be used to give data to the esp, eventually gets sent as a POST request to the dashboard.
  //Enable global interrupts
  sei();
  //Enable Timer 1 interrupt Flag register
  TIMSK1 |= (1 << TOIE1);

  //Set timer 1 Up in normal mode with a prescaler as 1024 and a top of xFFFF Where the overflow flag is set on xFFFF

  TCCR1A &= 0xFC; //Set WGM11 & WGM10 to 0 while keeping the rest of the reg set to its current value, Used to set normal mode
  TCCR1B |= 0x05; //Sets CS12 & CS10 to high for a prescaler of 1024
  TCCR1B &= 0xE5; //Clears WGM13 WGM12 to 0 and keeps CS12 & CS10 high as well as the rest of the reg

  //========Serial Setup========//

  Serial.begin(9600);
  esp.begin(9600);

  //========Pin Setup========//

  pinMode(lightPin, OUTPUT);

}

void loop()
{

  //Read as much as possible from the ESP8266 as append it to the command string.
  while (esp.available()) {
    char tempChar = esp.read();
    command += tempChar;
  }

  //------------------Command Decoder--------------------//
  //Note: We have to reset command in each of the conditionals due to serial coms being slower than the main loop.
  if (strcmp(command.c_str(), "toggleLights") == 0)
  {
    toggleLights(esp);
    command = "";
  }
  else if (strcmp(command.c_str(), "readTemp") == 0)
  {
    Serial.println("Reading Temp");
    command = "";
  }
  else if (strcmp(sendData.c_str(), "") != 0)
  {
    Serial.print("Data Being Sent Over: ");
    Serial.println(sendData.c_str());
    esp.write(sendData.c_str());
    sendData = "";
  }

}

//===================Command Functions===================//

//toggleLights: will toggle pin "lightPin" based on the lightOn variable.
void toggleLights(SoftwareSerial &esp)
{
  if (!lightOn)
  {

    esp.write("Should be High$k");
    Serial.println("Should Be High");
    digitalWrite(lightPin, HIGH);
  }
  else
  {
    esp.write("Should be Low$k"); //Use for nano
    Serial.println("Should Be Low");
    digitalWrite(lightPin, LOW);
  }
  lightOn = !lightOn;
}
//Timer interrupt every 4.19
ISR(TIMER1_OVF_vect) {
  //=====Serial for Ardiuno Nano========
  SoftwareSerial esp(7, 8);
  //esp.begin(9600);

  String lightStatus = "";
  //-------------Status Data--------------4
  //Get Light Status
  if (lightOn) {
    lightStatus = "ON";
  }
  else {
    lightStatus = "OFF";
  }

  String serverHeader = "lights=" + lightStatus + "$k";
  sendData = serverHeader;

  esp.print(serverHeader);
}
