# Teensy Octo

I am a cute monster who feeds off of your USB port.

## Hardware Specs

* Teensy Octo 2.0 microcontroller
* 8 pushbuttons
* 2 individually controller PWM RGB LEDs
* A Piezo Buzzer

## Serial Communication

The device communicates with the computer over serial. Information is sent in both direction:

* Octo -> PC: Button press / button release events
* PC -> Octo: LED / Buzzer control

## Python Wrapper Object

The `Octo` class in `octo.py` wraps serial communication into a easy-to-use object with methods to control the output devices and read switch state transactions.

## CLI Usage

The `cli.py` file provides a convinient way to control the Teensy via the command line.

```bash
$ python python/cli.py 
usage: cli.py [-h] [--address ADDRESS] [--led0 r g b] [--led1 r g b]
              [--buzz freq dur] [--command COMMAND] [--silence] [--reset]

A Command-Line control module for the Teensy Octo

optional arguments:
  -h, --help         show this help message and exit
  --address ADDRESS  Specify the serial interface address. Defaults to
                     /dev/ttyACM0
  --led0 r g b       Set LED 0 to the specified RGB levels (from range 0 -
                     255)
  --led1 r g b       Set LED 1 to the specified RGB levels (from range 0 -
                     255)
  --buzz freq dur    Activate the buzzer at the specified frequency (Hz) for
                     the specified duration (ms)
  --command COMMAND  Send a raw serial command string to the Octo
  --silence          Stop the buzzer
  --reset            Turn off both LEDs and the buzzer

Use those basic building blocks to conquer the world.
```

# Project Files

* octo.pdf - The schematic for the electronics, also available at CircuitLab
* teensy_octo.ino - C / Arduino compatible code for the firmware
* python/cli.py - A CLI wrapper for controlling the output devices on the Octo
* python/example.py - An example implementation of using the `Octo` library. Contains a `ButtonHandler` that reacts to button presses.
* python/test.py - A test suite for the output devices on the Octo
* python/octo.py - A Python wrapper for controlling the output devices and reading button state changes

# Known bugs

* The buzz function causes LED1 to light as RED.
* Sending serial commands too fast (before the device responds with the ready message) won't work

# Hacking

* Follow the guide to customize the firmware
* Write your own `ButtonHandler` classes
* Write a plugin-based architecture that'd allow multiple modules to run simultaneously

## Ideas

* A new Facebook message / Tweet / mail notifier
* Unittest runner with visual / audio feedback
* IO device for starting / stopping system (or remote) services
* HCI device for preprogrammed text input (can act as an external keyboard)
* Mood light
* Indicator for system load levels
* Digital authentication device for unlocking the screensaver
* Shortcut for userspace programs

# Authors

* Ando Roots [http://sqroot.eu] (programming, electronics)
* Valeria Gasik [http://sokeri.org] (design, sewing)