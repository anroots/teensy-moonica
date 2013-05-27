#!/usr/bin/env python

from octo import Octo
import time

# Continuously polls Octo button presses and calls methods from the ButtonHandler class
class ButtonListener:

    def __init__(self, octo, button_handler):
        """
        :param octo: Instance of the Octo class
        :param button_handler: Instance of a ButtonHandler class
        """
        self.button_handler = button_handler
        self.octo = octo
        self.octo.reset()

    def run(self):
        # Store previous read states (enables to detect falling/rising edges, we only want to do some action once)
        previous_states = self.octo.read_buttons()

        while True:
            time.sleep(0.3)
            current_states = self.octo.read_buttons()

            for button_number in current_states:

                # Determine the state of the button - either being pressed or released
                if previous_states[button_number] is Octo.BUTTON_OPEN \
                    and current_states[button_number] is Octo.BUTTON_CLOSED:
                    state_name = "press"
                elif previous_states[button_number] is Octo.BUTTON_CLOSED \
                    and current_states[button_number] is Octo.BUTTON_OPEN:
                    state_name = "release"
                else:  # No buttons pressed or released
                    continue

                # Call the handler method for this button/event
                handler_method_name = "button_%s_%s" % (str(button_number), state_name)
                if (hasattr(self.button_handler, handler_method_name)):
                    getattr(self.button_handler, handler_method_name)()
                else:
                    print "No handler for button %s state %s" % (str(button_number), state_name)

                previous_states[button_number] = current_states[button_number]