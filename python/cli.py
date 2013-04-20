#!/usr/bin/env python

# A CLI wrapper for the Teensy Octo.
# Allows to control the output devices of the Octo via the command line.

import argparse
import sys
from octo import Octo

def main(args):
    """
    Parse CLI args and pass them to the Octo library
    :param args: CLI arguments, parsed by the argparse library
    """
    octo = Octo(args.address)
    print args
    if '--buzz' in sys.argv:
        octo.buzz(args.buzz[0], args.buzz[1])
    elif '--silence' in sys.argv:
        octo.nobuzz()
    elif '--led0' in sys.argv:
        octo.led0(args.led[0],args.led[1],args.led[2])
    elif '--led1' in sys.argv:
        octo.led1(args.led[0],args.led[1],args.led[2])

# Init
if __name__ == '__main__':

    # When no arguments, show help
    if not len(sys.argv) > 1:
        sys.argv = ('cli.py', '-h')

    parser = argparse.ArgumentParser(description='A Command-Line control module for the Teensy Octo',
                                     epilog='Use those basic building blocks to conquer the world.')

    parser.add_argument('--address', nargs=1,
                        help='Specify the serial interface address. Defaults to /dev/ttyACM0',
                        default='/dev/ttyACM1')

    parser.add_argument('--led0', nargs=3, type=int, metavar=('r','g','b'), dest='led',
                    help='Set LED 0 to the specified RGB levels (from range 0 - 255)')

    parser.add_argument('--led1', nargs=3, type=int, metavar=('r','g','b'), dest='led',
                    help='Set LED 1 to the specified RGB levels (from range 0 - 255)')

    parser.add_argument('--buzz', nargs=2, type=int,
                        help='Activate the buzzer at the specified frequency (Hz) for the specified duration (ms)',
                        metavar=('freq','dur'))

    parser.add_argument('--silence', action='store_true',
                        help='Stop the buzzer')

    main(parser.parse_args())