#include "motor_driver.h"

motor_driver::motor_driver(){ }


void motor_driver::begin(int SPEED_PIN, int DIRECTION_PIN){

 speedPin = SPEED_PIN; //right, store on left
 directionPin = DIRECTION_PIN;

}


void motor_driver::setSpeed(int speed){
	
	if(speed > 0){

		digitalWrite(directionPin, HIGH);
	}
	
	else{
	digitalWrite(directionPin, LOW);
	}
	
	
	speed = abs(speed); //convert speed to positive for analogWrite
	analogWrite(speedPin, speed);
}