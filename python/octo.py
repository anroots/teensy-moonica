import serial
import sys
import time


class Octo:

    BUTTON_PRESSED = 1
    BUTTON_RELEASED = 2
    DELAY = 0.4

    def __init__(self, address):
        try:
            self.ser = serial.Serial(address, 9600, timeout=3)
        except:
            print "Couldn not open address %s" % address
            sys.exit(1)

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
        command = "%s %s %s %s" % ("led"+str(num), r, g, b)
        self.ser.write(command)

    def nobuzz(self):
        """
        Stop playing a tone through the buzzer
        """
        self.ser.write("nobuzz")

    def read_button(self, button_number):
        """
        Try to read a button event from the serial line

        :param button_number: Button number to read, 1..8
        :return: Either BUTTON_PRESSED, BUTTON_RELEASED constants or False if the event was not found
        """
        serial_data = self.read_raw().strip()
        if len(serial_data) == 0:
            return False

        if serial_data == "buttonPress: " + str(button_number):
            return self.BUTTON_PRESSED
        if serial_data == "buttonRelease: " + str(button_number):
            return self.BUTTON_RELEASED
        return False

    def read_raw(self):
        """
        Get a line from the serial interface
        :return:
        """
        return self.ser.readline().strip('\n')

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
        if hasattr(self,'ser'):
            self.ser.close()