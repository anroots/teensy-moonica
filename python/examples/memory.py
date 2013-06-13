#!/usr/bin/env python

# A simple memory game example.
# The Octo plays a sequence of notes and you have to repeat them.
# The legs act as a keyboard.


import os, sys
from random import randint
import time

# Hack to import from a parent dir
# http://stackoverflow.com/a/11158224/401554
parentdir = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, parentdir)

from listener import ButtonListener
from octo import Octo


def main():
    octo = Octo('/dev/ttyACM0')
    octo.reset()

    handler = ButtonHandler(octo)
    listener = ButtonListener(octo, handler, False)

    listener.run()


# This class defines methods for handling button events
#
# Method names follow this format: "button_<number>_<event>
# where number is in range 1..8 and event is either "press" or "release"
class ButtonHandler:
    TONE_C0 = 3830    # 261 Hz
    TONE_D = 3400    # 294 Hz
    TONE_E = 3038    # 329 Hz
    TONE_F = 2864    # 349 Hz
    TONE_G = 2550    # 392 Hz
    TONE_A = 2272    # 440 Hz
    TONE_B = 2028    # 493 Hz
    TONE_C1 = 1912    # 523 Hz

    # Define our 8-note scale of one octave
    TONE_MAP = [
        TONE_C0,
        TONE_D,
        TONE_E,
        TONE_F,
        TONE_G,
        TONE_A,
        TONE_B,
        TONE_C1
    ]

    TONE_DURATION = 200  # ms

    # Saves user entered notes as numbers in range 0..7
    user_sequence = []

    # Generated by Octo, compared against user sequence, items from 0..7
    correct_sequence = []

    def __init__(self, octo):
        self.octo = octo
        self.reset()

    def button_0_press(self):
        print "C0 "
        self.check(0)

    def button_1_press(self):
        print "D "
        self.check(1)

    def button_2_press(self):
        print "E "
        self.check(2)

    def button_3_press(self):
        print "F "
        self.check(3)

    def button_4_press(self):
        print "G "
        self.check(4)

    def button_5_press(self):
        print "A "
        self.check(5)

    def button_6_press(self):
        print "B "
        self.check(6)

    def button_7_press(self):
        print "C1 "
        self.check(7)

    def reset(self):
        time.sleep(Octo.DELAY * 3)

        self.user_sequence = []
        self.correct_sequence = [randint(0, 7), randint(0, 7), randint(0,7), randint(0,7)]

        print "Correct sequence is: %s" % self.correct_sequence

        for tone in self.correct_sequence:
            self.octo.buzz(self.TONE_MAP[tone], self.TONE_DURATION)
            time.sleep(Octo.DELAY)

        self.octo.reset()

    def check(self, position):

        self.octo.buzz(self.TONE_MAP[position], self.TONE_DURATION)

        time.sleep(Octo.DELAY)
        self.octo.led0(255, 255, 0)
        self.user_sequence.append(position)

        if len(self.user_sequence) is len(self.correct_sequence):
            if self.user_sequence == self.correct_sequence:
                red = 255
                green = 0
                print "Correct!"
            else:
                red = 0
                green = 255
                print "Wrong!"

            self.octo.led0(red, green, 255)
            time.sleep(Octo.DELAY)
            self.octo.led1(red, green, 255)
            time.sleep(3)

            self.reset()


if __name__ == '__main__':
    main()
