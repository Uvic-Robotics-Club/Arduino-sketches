## joystick_demo.py
This program will take in values from the silver USB joystick, convert them into the left/right speeds and directions
for the motors attached to the Arduino, and then send them to the Arduino.  

## runt_demo.ino
This program will recieve values over serial in the format:
_____________
### Arduino Communication Protocol 
Needs to be followed for all packages sent to the arduinos with the format "<(mode)|(data)>". </br> A sample data package is as follows: <S|60,-90>

|Mode|Mode Description|Data|Data Description|
|:--:|:--|:--:|:--|
|"I"|Request an identification of the arduino|id|Just a normal string|
|"S"|Set the motor speed|left_motors, right_motors|Two integers between the range [-100,100] separated by a comma|

*More modes to be added*
____________

The left and right speeds are values [-100, 100] where a negative value specifies the reverse direction. The start marker
is the '>' character. The seperator is a comma. The end marker is the '<' character.

## Instructions
1. [Install pygame](https://www.pygame.org/wiki/GettingStarted). This repo was tested with pygame version 1.9.6 and Python version 3.8.1 

2. If the raspberry pi is being used, make sure you run the python script with python3

3. This is currently set up for a baud rate of 9600. If you wish to change it, update the `BAUD_RATE` variable in both
runt_demo.ino and in joystick_demo.py. Make sure they are the same value.  

## Troubleshooting
If the runt rover appears to not map to what the joystick is doing, then stop joystick_demo.py, press the 
restart button on the Arduino, and restart joystick_demo.py