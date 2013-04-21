#!/usr/bin/env python
"""
This example demonstrates listening to button presses and taking appropriate actions
based on which button was pressed
"""

import time
import urllib2
from octo import Octo

def main():

    # Initialize the Octo library
    octo = Octo('/dev/ttyACM0')
    octo.reset()

    # Initialize the button handler class
    button_handler = ButtonHandler(octo)

    while True:
        time.sleep(0.3)

        # Try to read a button event
        button_event = octo.read_buttons()

        if button_event is False:  # No button is currently being pressed / released
            continue

        button_number =  button_event[0]  # 1..8
        button_state = button_event[1]  # Either Octo.BUTTON_PRESSED or Octo.BUTTON_RELEASED
        state_name = "press" if button_state is Octo.BUTTON_PRESSED else "release"

        handler_method_name = 'button_' +str(button_number)+"_"+state_name  # Call this function inside the button handler class
        if (hasattr(button_handler,handler_method_name)):
            getattr(button_handler, handler_method_name)()
        else:
            print "No handler for button "+str(button_number)+" state "+str(button_state)

# This class defines methods for handling button events
#
# Method names follow this format: "button_<number>_<event>
# where number is in range 1..8 and event is either "press" or "release"
class ButtonHandler:

    def __init__(self, octo):
        self.octo = octo

    def button_1_press(self):
        print "Button 1 pressed!"

    def button_1_release(self):
        print "Button 1 released!"

    def button_2_press(self):
        try:
            connection = urllib2.urlopen('http://waher.net')
            if connection.getcode() is 200:
                print "waher.net is up"
                self.octo.led0(255,0,255)
                connection.close()
        except urllib2.HTTPError, e:
            print "waher.net is down (%s)" % e.getcode()
            self.octo.led1(0,255,255)

    def button_3_press(self):
        for i in range(0,50):
            self.octo.led0(255,255,255-5*i)
            time.sleep(0.1)
        self.octo.reset()
        print "Done"

if __name__ == '__main__':
    main()