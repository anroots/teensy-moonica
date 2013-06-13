#!/usr/bin/env python
import time
import sys
import os

# Hack to import from a parent dir
# http://stackoverflow.com/a/11158224/401554
parentdir = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, parentdir)

from octo import Octo

octo = Octo('/dev/ttyACM0')
octo.reset()

while True:
    print octo.read_buttons()
    time.sleep(0.3)
