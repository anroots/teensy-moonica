#!/usr/bin/env python

# Push a repo to remote when a specific button is pressed

import os, sys

# Hack to import from a parent dir
# http://stackoverflow.com/a/11158224/401554
parentdir = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, parentdir)

from listener import ButtonListener
from octo import Octo
from subprocess import call



def main():
    octo = Octo('/dev/ttyACM0')
    octo.reset()

    handler = ButtonHandler(octo)
    listener = ButtonListener(octo, handler)

    print "Standby..."
    listener.run()


# This class defines methods for handling button events
#
# Method names follow this format: "button_<number>_<event>
# where number is in range 1..8 and event is either "press" or "release"
class ButtonHandler:
    def __init__(self, octo):
        self.octo = octo

    def button_0_press(self):
        print "Pushing sienna to remote..."
        os.chdir('/work/rec/sienna')
        out = call(['git', 'push'])
        if out is not 0:
            self.octo.led1(0,255,255)
        else:
            self.octo.led1(255,0,255)        
        print "OK"


if __name__ == '__main__':
    main()
