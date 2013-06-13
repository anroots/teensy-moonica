#!/usr/bin/env python

"""
Test suite for the Teensy Octo Python library
"""
import os

import time
import sys

# Hack to import from a parent dir
# http://stackoverflow.com/a/11158224/401554
parentdir = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, parentdir)

from octo import Octo

octo = Octo('/dev/ttyACM0')


def main():
    print "Test suite started"
    reset()
    hold()

    print "Testing LED-s"
    for i in range(0, 2):

        print "LED %s, Red" % i
        hold()
        octo.led(0, 255, 255, i)
        hold()
        print "LED %s, Green" % i
        octo.led(255, 0, 255, i)
        hold()
        print "LED %s, Blue" % i
        octo.led(255, 255, 0, i)
        hold()

        for r in range(0, 255):
            sys.stdout.write("LED %s, RED: %s%%\r" % (i, r * 100 / 255))
            sys.stdout.flush()
            octo.led(255 - r, 255, 255, i)
            time.sleep(0.01)
        print ""

        for g in range(0, 255):
            sys.stdout.write("LED %s, GREEN: %s%%\r" % (i, g * 100 / 255))
            sys.stdout.flush()
            octo.led(255, 255 - g, 255, i)
            time.sleep(0.01)
        print ""

        for b in range(0, 255):
            sys.stdout.write("LED %s, BLUE: %s%%\r" % (i, b * 100 / 255))
            sys.stdout.flush()
            octo.led(255, 255, 255 - b, i)
            time.sleep(0.01)
        print ""

        reset()

    print "Testing buzzer"
    for i in range(0, 10000, 1000):
        octo.buzz(i, 100)
        time.sleep(octo.DELAY)

    print "Testing buttons"
    for i in range(0, 8):
        sys.stdout.write("Press button %s..." % i)
        sys.stdout.flush()

        while not octo.read_button(i):
            time.sleep(0.1)
        print " OK"

    print "Test suite completed"
    reset()


def reset():
    octo.reset()


def hold():
    #raw_input("[press ENTER]")
    time.sleep(0.2)


if __name__ == '__main__':
    main()