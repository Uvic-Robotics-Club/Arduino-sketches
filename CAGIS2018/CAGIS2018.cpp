#include "CAGIS2018.h" //this is CRITICAL

//initializes the object
//the actual setup happens in the begin() function
CAGIS2018::CAGIS2018(){
}

//Set up the object
void CAGIS2018::begin(){
  //Seed the random sequence off of unused analog inputs
  randomSeed(analogRead(3)*analogRead(4)*(analogRead(POT)+analogRead(5)));
  //Generate a new random combination
  getCombo();
  //Start at the first number
  currStep=0;
}

//Flash the green LED to indicate a correct guess
//Also, increment currStep
void CAGIS2018::correct(){
  //Move on to next slot
  currStep++;
  //Print (diagnostic)
  Serial.println("CORRECT");
  //Flash green LED
  for (int i = 0; i < 3; i++){
    digitalWrite(GREEN, 255);
    delay(100);
    digitalWrite(GREEN, 0);
    delay(100);
  }
}

//Flash the red LED to indicate an incorrect guess
//Also, reset currStep
void CAGIS2018::incorrect(){
  currStep=0;
  //Print (diagnostic)
  Serial.println("INCORRECT");
  //Flash red LED
  for (int i = 0; i < 3; i++){
    digitalWrite(RED, 255);
    delay(100);
    digitalWrite(RED, 0);
    delay(100);
  }
}

//Turn on the green LED and stall here forever
void CAGIS2018::finish(){
  digitalWrite(GREEN, 255);
  Serial.println("Well done!");
  while(true);
}

//Generate a new random combination
void CAGIS2018::getCombo(){
  for (int n = 0; n < 3; n++){
    combo[n] = random(MARGIN, 1023-MARGIN);
  }
}

//Try the current pot position
void CAGIS2018::guess(){
  Serial.print("Your guess is ");
  if (bingo()){
    correct();
  }
  else{
    incorrect();
  }
  //If all three numbers have been guessed, finish the program!
  if(currStep>2){
    finish();
  }
}

//Get a voltage value between 0 and 1023
//(between 0V and 5V)
int CAGIS2018::readPot(){
  return analogRead(POT);
}

//Determines whether you've made a correct guess
boolean CAGIS2018::bingo(){
  reading = readPot();
  return (reading >= combo[currStep]-MARGIN && reading <= combo[currStep]+MARGIN);
}

