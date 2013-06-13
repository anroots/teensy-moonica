#!/usr/bin/env python
"""
Test buttons by switching LED colors
"""
import os
import time
import sys

# Hack to import from a parent dir
# http://stackoverflow.com/a/11158224/401554
parentdir = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, parentdir)

from octo import Octo
from listener import ButtonListener


def main():
    octo = Octo('/dev/ttyACM0')
    octo.reset()

    handler = ButtonHandler(octo)
    listener = ButtonListener(octo, handler)

    print "Press any of the Octo buttons. Press Ctrl + C to exit."
    listener.run()

# This class defines methods for handling button events
#
# Method names follow this format: "button_<number>_<event>
# where number is in range 1..8 and event is either "press" or "release"
class ButtonHandler:
    def __init__(self, octo):
        self.octo = octo

    def button_0_press(self):
        self.octo.led0(255, 0, 0)
        time.sleep(Octo.DELAY)
        self.octo.led1(0, 255, 0)

    def button_1_press(self):
        self.octo.led0(0, 255, 0)
        time.sleep(Octo.DELAY)
        self.octo.led1(0, 0, 255)

    def button_2_press(self):
        self.octo.led0(0, 0, 255)
        time.sleep(Octo.DELAY)
        self.octo.led1(255, 255, 0)

    def button_3_press(self):
        self.octo.led0(255, 255, 0)
        time.sleep(Octo.DELAY)
        self.octo.led1(255, 0, 255)

    def button_4_press(self):
        self.octo.led0(255, 0, 255)
        time.sleep(Octo.DELAY)
        self.octo.led1(0, 255, 255)

    def button_5_press(self):
        self.octo.led0(0, 255, 255)
        time.sleep(Octo.DELAY)
        self.octo.led1(255, 255, 255)

    def button_6_press(self):
        self.octo.led0(255, 255, 255)
        time.sleep(Octo.DELAY)
        self.octo.led1(0, 0, 0)

    def button_7_press(self):
        self.octo.led0(0, 0, 0)
        time.sleep(Octo.DELAY)
        self.octo.led1(255, 0, 0)


if __name__ == '__main__':
    main()