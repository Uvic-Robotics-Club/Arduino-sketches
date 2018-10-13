#include "motor_driver.h"
motor_driver motorLeft;
motor_driver motorRight;
//MOTOR--------------------


#include <SPI.h>
#include "RF24.h"
RF24 myRadio (7, 8);
byte addresses[][6] = {"0"};
struct package {
  int leftSpeed = 0;
  int rightSpeed = 0;
};
typedef struct package Package;
Package data;
//RADIO---------------------------


void setup() {
  motorLeft.begin(3, 2); //speed pin, direction pin
  motorRight.begin(5, 8);

  myRadio.begin();
  myRadio.setChannel(115);
  myRadio.setPALevel(RF24_PA_MAX);
  myRadio.setDataRate( RF24_250KBPS ) ;
  myRadio.openReadingPipe(1, addresses[0]);
  myRadio.startListening();
}


void loop() {
  if (myRadio.available()) {
    while (myRadio.available()) {
      myRadio.read( &data, sizeof(data) );
    }
    motorLeft.setSpeed(data.leftSpeed);
    motorRight.setSpeed(data.rightSpeed);
  }
}
