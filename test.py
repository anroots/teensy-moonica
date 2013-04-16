#!/usr/bin/env python

# Test suite for the Teensy Octo Python library

import time
import sys

from octo import Octo

octo = Octo('/dev/ttyACM0')


def main():
    print "Test suite started"
    reset()
    hold()

    print "Testing LED-s"
    for i in range(0, 2):

        print "Individual RGB colors" % i
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

        print "Color combinations" % i
        for r in range(0, 255):
            sys.stdout.write("LED %s, RED: %s%%\r" % (i, r * 100 / 255))
            sys.stdout.flush()
            octo.led(255 - r, 255, 255, i)
            time.sleep(0.01)

        reset()

    # Done
    for i in range(0, 3):
        octo.buzz(3000, 100)
        time.sleep(0.500)

    print "Test suite completed"
    reset()

def reset():
    octo.reset()


def hold():
    #raw_input("[press ENTER]")
    time.sleep(0.4)


if __name__ == '__main__':
    main()