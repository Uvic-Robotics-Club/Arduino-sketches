# joystick_demo.py
# This program will read the x and y axis data from a USB connected joystick and
# will convert that data into left/right speed values for the runt rover. Then it
# will send those 2 values to the runt rover over serial in the form: 
#
# <start marker><left speed integer><seperator><right speed integer><end marker>
#
# The left and right speeds are values [-100, 100] where a negative value
# specifies the reverse direction. The start marker is the '>' character. The
# seperator is a comma. The end marker is the '<' character.

import pygame
import serial
from time import time

SERIAL_PORT = 'COM4' # TODO: Update to use the correct port
BAUD_RATE = 9600

MAX_VAL = 100
X_AXIS_DEADZONE = 0.14 * MAX_VAL
Y_AXIS_DEADZONE = 0.14 * MAX_VAL
Z_AXIS_DEADZONE = 0.20 * MAX_VAL # Rotation around z axis
SLIDER_OFFSET = 1 * MAX_VAL # Slider in front of joystick

class joystick_demo:
	
    def main():
        pygame.init()
        ser = serial.Serial(SERIAL_PORT, BAUD_RATE)
        current_time = time()

        # Initialize joystick
        try:
        	joystick = pygame.joystick.Joystick(0)
        	joystick.init()
        except pygame.error:
        	print("Cannot find joystick. Not running joystick.")
        	return

        while pygame.joystick.get_count() > 0:
            if (time() - current_time) < 0.05:
                continue
            current_time = time()

            # Retrieve joystick data
            # X and Y axis is range [-100.0, 100.0] where negative is reverse
            pygame.event.get()
            x_axis = joystick.get_axis(0) * MAX_VAL
            y_axis = (0 - joystick.get_axis(1)) * MAX_VAL

            # Rotation around Z axis is range [-100.0, 100.0] where negative is left rotation
            z_axis = joystick.get_axis(3) * MAX_VAL
            
            # Limiter is in range [0, 10.0] where 0 is when slider is all the way
            # back towards negative sign
            limiter = (0 - joystick.get_axis(2)) * MAX_VAL
            limiter = (limiter + SLIDER_OFFSET) / 2 # Maps from [-100.0, 100.0] to [0, 100.0]

            # Set initial speed before considering turning
            speedLeft = speedRight = y_axis

            if abs(x_axis) < X_AXIS_DEADZONE and abs(y_axis) < Y_AXIS_DEADZONE:
                # Joystick is centred
                if abs(z_axis) > Z_AXIS_DEADZONE:
                    # Rotate rover in place
                    speedLeft += z_axis
                    speedRight -= z_axis
            else:
                # Joystick is not centred
                if x_axis != 0:
                    # Turn rover
                    speedLeft += x_axis
                    speedRight -= x_axis

            # Limit values
            speedLeft = -limiter if speedLeft < -limiter else limiter if speedLeft > limiter else speedLeft
            speedRight = -limiter if speedRight < -limiter else limiter if speedRight > limiter else speedRight

            # Arduino expects ints
            speedLeft = int(speedLeft)
            speedRight = int(speedRight)

            packet = ">" + str(speedLeft) + "," + str(speedRight) + "<"
            print(packet)
            ser.write(packet.encode())

    if __name__ == '__main__':
        main()