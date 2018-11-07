#include "OODemo.h" //this is CRITICAL

//initializes the object
//the actual setup happens in the begin() function
OODemo::OODemo(){
}

//Set up the object
void OODemo::begin(){
  String tmpchar = "d";
  setDir(tmpchar); //start in forward direction
  
  setSpeed(0); //start motionless
  //(though this should be immediately overruled by analog input)
}

void OODemo::setSpeed(int spd){
  //get the speed from the 0-1023 scale to the 0-100 scale
  //which our PWM setup requires
  speed = spd/10;
  Serial.println(speed);
}

void OODemo::setDir(String dir){
  if (dir == "d" || dir == "D"){
    digitalWrite(DIR, HIGH);
  }
  if (dir == "r" || dir == "R"){
    digitalWrite(DIR, LOW);
  }
}

void OODemo::updateMotor(){
  // Run on duty cycles of 100 milliseconds
  // Note: Integers on arduinos are only 2 bytes. This means that when counting time in milliseconds,
  // it will overflow at just over 30 seconds. Using a long increases this to about 50 days
  long t = millis();
  // Mod of a negative number is negative in Arduino (if not 0). Cycle time MUST be positive
  // This is only technically needed when t is negative (at times <50 days), but is good practice
  int cycleTime = abs(t % 100);
  
  if (cycleTime < speed)
      digitalWrite(PWM, 255);
  else
      digitalWrite(PWM, 0);
}

