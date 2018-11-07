//Header file!
//These include all the declarations for the code file
//and provides declarations for different code files
//so that they know that other code files' functions
//exist, enabling them to call those functions

//These (and the #endif at the bottom) prevent this
//from being included more than once in a project
#ifndef CAGIS2018_h
#define CAGIS2018_h

// Digital pins for LEDs
#define RED 3
#define GREEN 5

// Analog input pin for potentiometer
#define POT A0

// Digital input pin for button
#define BUTTON 9

// Determines the precision with which one must guess numbers
#define MARGIN 100

//contains many often-used utility functions
#include "Arduino.h"

class CAGIS2018{

  private:
    //stuff here CANNOT be accessed by the .ino

    //tracks which number you're trying to crack
    int currStep;
    //temporarily stores the last reading of the pot
    int reading;

    void correct();
    void incorrect();
    void finish();
    void getCombo();
    int readPot();
    boolean bingo();
  
  public:
    //stuff here CAN be accessed by the .ino

    CAGIS2018(); //Constructor declaration

    //Random numbers for lock combination
    int combo [3];

    void begin();
    void guess();
};

//everything before the #endif is neutralized if the
//#ifndef variable is defined
#endif
