/*
 * The .ino file INSTANTIATES instances of the objects defined in the 
 * .cpp and RUNS the .cpp's functions.
 * 
 */

#include "OODemo.h" //this is CRITICAL

//Declare an INSTANCE of the OODemo object
OODemo board;
int tmpreading;

void setup() {
  board.begin();

  //Initialize serial communication with baud rate 9600Hz
  Serial.begin(9600);

  //Set up digital pins for output
  pinMode(PWM, OUTPUT);
  pinMode(DIR, OUTPUT);
}

void loop(){
  board.updateMotor();

  tmpreading = analogRead(ENCODER);
  //Print the pot's output to the serial monitor
  //Serial.println(tmpreading);
  //Update speed based on the reading
  board.setSpeed(tmpreading);
}

//serialEvent is called whenever new data comes in through the Serial port
//we want to be able to change direction by entering
//"d" or "r" (drive or reverse) through the serial monitor
void serialEvent(){
  //Change direction if needed
  board.setDir(Serial.readString());
}

