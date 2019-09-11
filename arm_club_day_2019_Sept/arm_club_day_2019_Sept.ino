/* This is a program for controlling the two linear actuators on the robotic arm
 * Takes input from serial: <upper speed int> <lower speed int> 314
 *    Upper speed and lower speed are expected to be in the range [-100, 100].
 *    Numbers outside of this range will be treated as the maximum
 * Will deactivate all movement if too long passes before receiving another update
 * Author: Gregory O'Hagan
 */

#include <Stepper.h>

//Define pinout for board
#define ROLL_STEPPER_DIR 8
#define ROLL_STEPPER_PULSE 9
#define ROLL_POT A0

#define PITCH_STEPPER_DIR 2
#define PITCH_STEPPER_PULSE 3
#define PITCH_POT A1

#define CLAW_DIR 5
#define CLAW_SPEED 6
#define CLAW_ENDSTOP 10

#define INPUT_KEY 315

// Initialize global control variables
int rollSpeed = 0;
int rollDirection = 0;
int cyclesSinceLastRollClick = 0;

int pitchSpeed = 0;
int pitchDirection = 0;
int cyclesSinceLastPitchClick= 0;

int clawSpeed = 0;
int clawDirection = 0;

long lastUpdate = 0;
long lastCycle = 0;

int badlines = 0;

// Setup code, runs once
void setup() {
 Serial.begin(9600);
 
  pinMode(ROLL_STEPPER_DIR, OUTPUT);
  pinMode(ROLL_STEPPER_PULSE, OUTPUT);
  pinMode(PITCH_STEPPER_DIR, OUTPUT);
  pinMode(PITCH_STEPPER_PULSE, OUTPUT);
  pinMode(CLAW_DIR, OUTPUT);
  pinMode(CLAW_SPEED, OUTPUT);
  pinMode(CLAW_ENDSTOP, INPUT);
}

// Main code, rus repeatedly
void loop() {
  update_motors();
//  Serial.print(analogRead(ROLL_POT));
//  Serial.print(" ");
//  Serial.println(analogRead(PITCH_POT));
//  Serial.println(badlines);
}


// Update the two linear actuators, using global values
void update_motors(){
  // Run on cycles of 100 milliseconds
  // Note: Integers on arduinos are only 2 bytes. This means that when counting time in milliseconds,
  // it will overflow at just over 30 seconds. Using a long increases this to about 50 days
  long t = millis();
  // Mod of a negative number is negative in Arduino (if not 0). Cycle time MUST be positive
  // This is only technically needed when t is negative (at times <50 days), but is good practice
  int cycleTime = abs(t % 100);
  int newCycle = 1;
  if ((t % 100) == (lastCycle % 100))
    newCycle = 0;
  lastCycle = t;

  // Stop everything if we haven't heard an update recently
  if (t - lastUpdate > 100){
    analogWrite(CLAW_SPEED, 0);
  }
  // If cycleTime is less than the speed value, turn it on. Otherwise, turn it off.
  // This gives pulses (note: not actually a pwm) of variable length to control how quickly the arm moves
  // Using a simple pwm is unreliable at lower strengths, and cannot do fine controls (due to inductance/friction in motors)
  else {
    digitalWrite(CLAW_DIR, clawDirection);
    analogWrite(CLAW_SPEED, clawSpeed);

    // Convoluted speed control for steppers
    if (newCycle == 1){
      if (cyclesSinceLastRollClick < 100)
        cyclesSinceLastRollClick += 1;
      if (newCycle && abs(rollSpeed) > (100 - cyclesSinceLastRollClick)){
        digitalWrite(ROLL_STEPPER_DIR, rollDirection);
        digitalWrite(ROLL_STEPPER_PULSE, HIGH);
        digitalWrite(ROLL_STEPPER_PULSE, LOW);
        cyclesSinceLastRollClick = 0;
      }
      
      if (cyclesSinceLastPitchClick < 100)
        cyclesSinceLastPitchClick += 1;
      if (newCycle && abs(pitchSpeed) > (100 - cyclesSinceLastPitchClick)){
        digitalWrite(PITCH_STEPPER_DIR, pitchDirection);
        digitalWrite(PITCH_STEPPER_PULSE, HIGH);
        digitalWrite(PITCH_STEPPER_PULSE, LOW);
        cyclesSinceLastPitchClick = 0;
      }
    }
  }
}


// Runs once between each call of loop. Parses serial data if available,
// Updates lastUpdate to indicate it received new information
void serialEvent(){
  if (Serial.available() > 1){
    int speed1;
    int speed2;
    int speed3;
    // Grab integers from serial.
    // Note that if we ever receive the "key", we return immediately.
    // This will (hopefully) get us back in sync if we ever get out of sync
    speed1 = Serial.parseInt();
    if (speed1 == INPUT_KEY)
      return;
    speed2 = Serial.parseInt();
    if (speed2 == INPUT_KEY)
      return;
    speed3 = Serial.parseInt();
    if (speed3 == INPUT_KEY)
      return;
    int checkSum = Serial.parseInt();
    if (checkSum != speed1 * 100 + speed2 * 10 + speed3)
      return;
    int key = Serial.parseInt();
    // Ensure there isn't any issues with the data. If the key doesn't match, don't update the data
    if (!(key == INPUT_KEY)){
//      delay(5);
      return;
    }
//    delay(5);
    // Positive/negative indicates direction, abs. value indicates speed
    if (speed1 < 0){
      rollSpeed = 0 - speed1;
      rollDirection = 0;
    }
    else{
      rollSpeed = speed1;
      rollDirection = 1;
    }

    if (speed2 < 0){
      pitchSpeed = 0 - speed2;
      pitchDirection = 0;
    }
    else{
      pitchSpeed = speed2;
      pitchDirection = 1;
    }

    if (speed3 < 0){
      clawSpeed = 0 - speed3;
      clawDirection = 0;
    }
    else{
      clawSpeed = speed3;
      clawDirection = 1;
    }

    // Record the time of the most recent successful data read (for timeout logic)
    lastUpdate = millis();
//    if((clawSpeed != 0) || pitchSpeed != 0 || rollSpeed != 0)
//      badlines ++;
  }
}
