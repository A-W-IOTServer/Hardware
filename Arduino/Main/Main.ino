//--------------------Imports-----------------------
#include <SoftwareSerial.h>
//---------------------Pin Setup------------------------

const int finishedInterruptPin = 3;
const int lightPin = 52;

//--------------Globals-----------------

double debounceTime = 0; //Set up to debounce the trigger that comes from the esp. This should stop mutiple triggers that come from the esp.
bool commandTrigger = false; //keeps track if someone hits the endpoint.
bool lightOn = false;

void setup() {

  Serial.begin(9600);
  Serial1.begin(9600);
  pinMode(lightPin, OUTPUT);
}

void loop() {
  
  if(Serial1.available()){
    char command = Serial1.read();
    if(command == 'L'){
      Serial.println("Light Command Found!");
      commandTrigger = true;
    }
  }
  if(commandTrigger){
    if (!lightOn){
      Serial.println("Should Be High");
      digitalWrite(lightPin, HIGH);
    }
    else{
      Serial.println("Should Be Low");
      digitalWrite(lightPin, LOW);
    }
    lightOn = !lightOn;
    commandTrigger = false;
  }
}
