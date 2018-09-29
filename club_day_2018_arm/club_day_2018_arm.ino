/* This is a program for controlling the two linear actuators on the robotic arm
 * Takes input from serial: <upper speed int> <lower speed int> 314
 *    Upper speed and lower speed are expected to be in the range [-100, 100].
 *    Numbers outside of this range will be treated as the maximum
 * Will deactivate all movement if too long passes before receiving another update
 * Author: Gregory O'Hagan
 */

//Define pinout for board
#define UPPER_PWM 5
#define UPPER_DIR 6

#define LOWER_PWM 10
#define LOWER_DIR 11

// Initialize global control variables
int upperSpeed = 0;
int lowerSpeed = 0;
int upperDirection = 0;
int lowerDirection = 0;
long lastUpdate = 0;

// Setup code, runs once
void setup() {
 Serial.begin(9600);
 
 pinMode(UPPER_PWM, OUTPUT);
 pinMode(UPPER_DIR, OUTPUT);
 
 pinMode(LOWER_PWM, OUTPUT);
 pinMode(LOWER_DIR, OUTPUT);

}

// Main code, rus repeatedly
void loop() {
  update_motors();
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

  // Stop everything if we haven't heard an update within the last 100 milliseconds
  /*if (t - lastUpdate > 100){
    digitalWrite(UPPER_DIR, LOW);
    digitalWrite(LOWER_DIR, LOW);
    analogWrite(UPPER_PWM, 0);
    analogWrite(LOWER_PWM, 0);
  }*/
  // If cycleTime is less than the speed value, turn it on. Otherwise, turn it off.
  // This gives pulses (note: not actually a pwm) of variable length to control how quickly the arm moves
  // Using a simple pwm is unreliable at lower strengths, and cannot do fine controls (due to inductance/friction in motors)
  // else {
    digitalWrite(UPPER_DIR, upperDirection);
    digitalWrite(LOWER_DIR, lowerDirection);
    if (cycleTime < upperSpeed)
      analogWrite(UPPER_PWM, 255);
    else
      analogWrite(UPPER_PWM, 0);
    if (cycleTime < lowerSpeed)
      analogWrite(LOWER_PWM, 255);
    else
      analogWrite(LOWER_PWM, 0);
  //}
}


// Runs once between each call of loop. Parses serial data if available,
// Updates lastUpdate to indicate it received new information
void serialEvent(){
  if (Serial.available() > 1){
    // Grab 2 integers from serial
    lowerSpeed = Serial.parseInt();
    // upperSpeed = Serial.parseInt();
    // int key = Serial.parseInt();
    // Ensure there isn't any issues with the data. If rhe key doesn't match, don't update the data
    // if (!(key == 314))
    //  return;

    // Positive/negative indicates direction, abs. value indicates speed
    if (lowerSpeed < 0){
      lowerSpeed = 0 - lowerSpeed;
      lowerDirection = 0;
    }
    else{
      lowerDirection = 1;
    }

    if (upperSpeed < 0){
      upperSpeed = 0 - upperSpeed;
      upperDirection = 0;
    }
    else{
      upperDirection = 1;
    }

    // Record the time of the most recent successful data read (for timeout logic)
    lastUpdate = millis();
  }
}

