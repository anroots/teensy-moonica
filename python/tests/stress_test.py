#!/usr/bin/env python
import time
import sys
import os
from random import randint

# Hack to import from a parent dir
# http://stackoverflow.com/a/11158224/401554
parentdir = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, parentdir)

from octo import Octo

octo = Octo('/dev/ttyACM0')
octo.reset()

# Test that the LEDs don't burn out or short or what knot during continious active state
while True:
    time.sleep(1)
    octo.led0(randint(0,255),randint(0,255),randint(0,255))
    time.sleep(1)
    octo.led1(randint(0,255),randint(0,255),randint(0,255))
    time.sleep(randint(60,120))
    octo.led1(randint(0,255),randint(0,255),randint(0,255))
    time.sleep(1)
    octo.led0(randint(0,255),randint(0,255),randint(0,255))
    time.sleep(randint(60,120))
