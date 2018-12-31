//=======================Imports=======================//

#include <SoftwareSerial.h>
#include <String.h>

//=======================Pin Setup=======================//

const int lightPin = 22;

//=======================Globals=======================//

//---Integers---

//---Strings---
String command = "";

//---Booleans---
bool lightOn = false;

void setup()
{
  //========Serial Setup========//

  Serial.begin(9600);
  Serial1.begin(9600);

  //========Pin Setup========//

  pinMode(lightPin, OUTPUT);

}

void loop()
{

  //Read as much as possible from the ESP8266 as append it to the command string.
  while (Serial1.available()) {
    char tempChar = Serial1.read();
    command += tempChar;
  }

  //------------------Command Decoder--------------------//
  //Note: We have to reset command in each of the conditionals due to serial coms being slower than the main loop.
  if (strcmp(command.c_str(), "toggleLights") == 0)
  {
    toggleLights();
    command = "";
  }
  else if (strcmp(command.c_str(), "readTemp") == 0)
  {
    Serial.println("Reading Temp");
    command = "";
  }

}

//===================Command Functions===================//

//toggleLights: will toggle pin "lightPin" based on the lightOn variable.
void toggleLights()
{
  if (!lightOn)
  {
    Serial.println("Should Be High");
    digitalWrite(lightPin, HIGH);
  }
  else
  {
    Serial.println("Should Be Low");
    digitalWrite(lightPin, LOW);
  }
  lightOn = !lightOn;
}


