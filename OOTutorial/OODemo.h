//Header file!
//These include all the declarations for the code file
//and provides declarations for different code files
//so that they know that other code files' functions
//exist, enabling them to call those functions

#ifndef _OO_demo_h
#define _OO_demo_h

//Alternatively:
//#pragma once

//Define pinout for board
#define PWM 5
#define DIR 6
#define ENCODER A0

#include "Arduino.h" //need this for access to AnalogWrite

class OODemo{

  public:
    //stuff here CAN be accessed by the .ino

    OODemo(); //constructor
    void begin(); //instantiation function

    void setSpeed(int spd);
    void setDir(String dir);

    void updateMotor();
    
  private:
    //stuff here CANNOT be accessed by the .ino
    int speed;
};


#endif
