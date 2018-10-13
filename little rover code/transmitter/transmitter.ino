#include <SPI.h>
#include "RF24.h"
RF24 myRadio (7, 8);
byte addresses[][6] = {"0"};
struct package {
  int leftSpeed;
  int rightSpeed;
};
typedef struct package Package;
Package data;
//RADIO--------------------------------


void setup() {
  myRadio.begin();
  myRadio.setChannel(115);
  myRadio.setPALevel(RF24_PA_MAX); //highest transmitting power - longest range
  myRadio.setDataRate( RF24_250KBPS ); //lowest data transfer rate - longest range
  myRadio.openWritingPipe( addresses[0]);

  Serial.begin(115200);
  Serial.setTimeout(10);
}


void loop() {
  while (Serial.available() == 0) {}
  
  data.leftSpeed = Serial.parseInt();
  data.rightSpeed = Serial.parseInt();

  Serial.print(data.leftSpeed);
  Serial.print("\t");
  Serial.println(data.rightSpeed);

  myRadio.write(&data, sizeof(data));
}
