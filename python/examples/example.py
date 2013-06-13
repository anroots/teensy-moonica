#!/usr/bin/env python
"""
This example demonstrates listening to button presses and taking appropriate actions
based on which button was pressed
"""

import os, sys

# Hack to import from a parent dir
# http://stackoverflow.com/a/11158224/401554
parentdir = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, parentdir)

import time
import urllib2
from octo import Octo
from listener import ButtonListener


def main():
    octo = Octo('/dev/ttyACM0')
    octo.reset()

    handler = ButtonHandler(octo)
    listener = ButtonListener(octo, handler)

    listener.run()


# This class defines methods for handling button events
#
# Method names follow this format: "button_<number>_<event>
# where number is in range 1..8 and event is either "press" or "release"
class ButtonHandler:
    def __init__(self, octo):
        self.octo = octo

    def button_0_press(self):
        print "Button 1 pressed!"

    def button_0_release(self):
        print "Button 1 released!"

    def button_1_press(self):
        try:
            connection = urllib2.urlopen('http://waher.net')
            if connection.getcode() is 200:
                print "waher.net is up"
                self.octo.led0(255, 0, 255)
                connection.close()
        except urllib2.HTTPError, e:
            print "waher.net is down (%s)" % e.getcode()
            self.octo.led1(0, 255, 255)

    def button_7_press(self):
        for i in range(0, 50):
            self.octo.led0(255, 255, 255 - 5 * i)
            time.sleep(0.1)
        self.octo.reset()
        print "Done"


if __name__ == '__main__':
    main()