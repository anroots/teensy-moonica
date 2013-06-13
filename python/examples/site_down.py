#!/usr/bin/env python

# Check if a site is down and light one eye of the Octo. Run via cron.

import os, sys

# Hack to import from a parent dir
# http://stackoverflow.com/a/11158224/401554
parentdir = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, parentdir)

from octo import Octo
import urllib2


def main():
    octo = Octo('/dev/ttyACM0')
    octo.reset()

    if get_http_status('http://pixlr.com') is not 200:
        octo.led1(0, 255, 255)
    else:
        octo.led1(255, 0, 255)

# Return the HTTP status of a given URL or None on error
def get_http_status(url):
    response = None
    try:
        connection = urllib2.urlopen(url)
        response = connection.getcode()
    except urllib2.HTTPError, e:
        print e.getcode()

    return response


if __name__ == '__main__':
    main()