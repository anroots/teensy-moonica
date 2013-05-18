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

    # Store previous read states (enables to detect falling/rising edges, we only want to do some action once)
    previous_states = octo.read_buttons()

    while True:
        time.sleep(0.3)
        current_states = octo.read_buttons()

        for button_number in current_states:

            # Determine the state of the button - either being pressed or released
            if previous_states[button_number] is Octo.BUTTON_OPEN and current_states[
                button_number] is Octo.BUTTON_CLOSED:
                state_name = "press"
            elif previous_states[button_number] is Octo.BUTTON_CLOSED and current_states[
                button_number] is Octo.BUTTON_OPEN:
                state_name = "release"
            else:
                continue

            # Call the handler method for this button/event
            handler_method_name = "button_%s_%s" % (str(button_number), state_name)
            if (hasattr(button_handler, handler_method_name)):
                getattr(button_handler, handler_method_name)()
            else:
                print "No handler for button %s state %s" % (str(button_number), state_name)

            previous_states[button_number] = current_states[button_number]

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