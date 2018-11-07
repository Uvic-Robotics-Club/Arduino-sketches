#include "CAGIS2018.h"

//In Arduino, variables declared in the .ino are automatically
//instantiated when the .ino runs
CAGIS2018 safe;

int currButtonState = 0; //current state of button
int lastButtonState = 0; //previous state of button

//A setup() function runs once at the start of all Arduino programs
void setup(){
  //Set up the safe
  safe.begin();
  //Set up LED pins for output (not input)
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BUTTON, INPUT);

  //Just for debugging/demo purposes
  Serial.begin(9600);
}

//A loop() function repeats endlessly after the setup() function is
//done in all Arduino programs
void loop(){
  
  //Print the current reading to serial
  //Serial.println(analogRead(POT));
  
  //Read the button pin
  currButtonState=digitalRead(BUTTON);

  //Make a guess if the button has been freshly pressed
  if (lastButtonState==LOW && currButtonState==HIGH) {
    safe.guess();
    delay(50); //To avoid bouncing
  }
  lastButtonState=currButtonState;
  
}
