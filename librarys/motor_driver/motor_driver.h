#pragma once
#ifndef _motor_driver_h
#define _motor_driver_h
#include <Arduino.h>



class motor_driver{
 public:
 motor_driver();
 void begin(int SPEED_PIN, int DIRECTION_PIN);
 void setSpeed(int speed);
 
 
 private:
int speedPin;
int directionPin;


 
};
 
 
 
#endif