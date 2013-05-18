"""
A Python wrapper for the Teensy Octo project.

Exposes methods to read and write to the device over the serial interface.
"""

import serial
import sys
import time


class Octo:
    # Use those constants for button states
    BUTTON_CLOSED = True
    BUTTON_OPEN = False

    # How many ms to wait between commands (for not overflowing the buffers)
    DELAY = 0.4

    # Stores last known button states (dict, key=button number, val=button state constant)
    button_states = None

    def __init__(self, address):
        try:
            self.ser = serial.Serial(address, 9600, timeout=2)
        except:
            print "Could not open address %s" % address
            sys.exit(1)

        # Default value for all buttons - not pressed
        self.button_states = dict(zip(range(0, 8), [self.BUTTON_OPEN] * 8))

        # Allow to reset after a serial connection
        time.sleep(1)

        # Clear the buffer
        self.ser.flushInput()

    def buzz(self, freq, duration):
        """
        Play a sound through the Piezo buzzer
        :param freq: Sound frequency in Hz, don't go much higher than 0..10 000 (soft limit)
        :param duration: Sound duration in ms
        """
        self.ser.write("buzz %s %s" % (freq, duration))

    def led0(self, r, g, b):
        """
        Set LED0 to the specified RGB levels

        :param r: Red value, 0 - 255
        :param g: Green value, 0 - 255
        :param b: Blue value, 0 - 255
        """
        self.led(r, g, b, 0)

    def led1(self, r, g, b):
        """
        Set LED1 to the specified RGB levels

        :param r: Red value, 0 - 255
        :param g: Green value, 0 - 255
        :param b: Blue value, 0 - 255
        """
        self.led(r, g, b, 1)

    def led(self, r, g, b, num=0):
        """
        Power one of the RGB LED-s to the specified level

        :param r: Red value, 0 - 255
        :param g: Green value, 0 - 255
        :param b: Blue value, 0 - 255
        :param num: Either 0 or 1, specifies the LED to set
        """
        command = "%s %s %s %s" % ("led" + str(num), r, g, b)
        self.ser.write(command)

    def nobuzz(self):
        """
        Stop playing a tone through the buzzer
        """
        self.ser.write("nobuzz")

    def read_buttons(self):
        """
        Get information about the state of each of the device buttons

        :return: A dict of 8 values where keys are button numbers and values last known states
        """
        # No data sent via serial, use last known values
        if not self.ser.inWaiting():
            return self.button_states

        # Read a single byte from the serial buffer
        buttons_byte = self.ser.read(1)

        # Convert the byte into a binary string with a length of 8 (00101110)
        button_bits = str(bin(int(buttons_byte.encode('hex')[0:2], 16))[2:])

        # Zero-fill the bit-string (or the leading zeros would be lost)
        button_bits = button_bits.zfill(8)

        # Convert the bits into a list where the state of each button is represented by a boolean value
        button_states = map(lambda bit: bit == '1', button_bits)

        # Convert the list into a dict where button number is mapped to its state
        button_states = dict(zip(range(0, 8), button_states))

        # Save the reading for future use (when there is no serial input available from the device)
        self.button_states = button_states

        return button_states

    def read_button(self, button_number):
        """
        Get the status of the specified button (either open or closed)

        :param button_number: Button number, from 0..7
        :return: Either BUTTON_OPEN or BUTTON_CLOSED
        """
        return self.read_buttons()[button_number]

    def read_raw(self):
        """
        Get a line from the serial interface
        :return:
        """
        return self.ser.readline().strip('\n')

    def send_raw(self, string):
        """
        Send a raw serial command to the Octo
        :param string:
        """
        self.ser.write(str(string))

    def reset(self):
        """
        Reset (switch off) both LED-s and the buzzer
        """
        time.sleep(self.DELAY)
        self.nobuzz()
        time.sleep(self.DELAY)
        self.led0(255, 255, 255)
        time.sleep(self.DELAY)
        self.led1(255, 255, 255)
        time.sleep(self.DELAY)

    def __del__(self):
        """
        Close serial port on class destruction
        """
        if hasattr(self, 'ser'):
            self.ser.close()