# Teensy Octo

A custom-build USB IO device designed for software developers in mind.

Packaged as a cute eight-legged creature, this device connects to the USB port to do your bidding. Press any of the eight legs to activate pre-programmed commands or write software to one of its three output devices as an indicator of interesting events in the virtual world.

The hardware and software are open to hacking. The user has to program the macros for each button himself.

## Hardware Specs

* Teensy 2.0 microcontroller
* 8 pushbuttons
* 2 individually controller PWM RGB LEDs
* A Piezo Buzzer

### Initial hardware settings

* USB Type: Serial + Mouse + Keyboard + Joystick
* Board: Teensy 2.0
* CPU speed: 8 MHz
* Keyboard layout: US English

# Input

## As an external Input Device

The computer recognizes the Teensy as a gamepad/joystic type of device. By default it's programmed to send joystick button press events on each button press. Open the gamepad config window from your control panel to see the mapping.
Some 3rd party shortcut binder application can be used to map joystick events to useful actions on the PC - without writing any software.

## Serial Communication

The device communicates with the computer over serial. Information is sent in both direction:

* Octo -> PC: Button state changes
* PC -> Octo: LED / Buzzer control

The Teensy sends a byte over serial each time one of its button states change. The byte represents all (current) button states at once (`01000001` means that the second and the last button are currently pressed). 

# Output

Serial commands can be sent to the device using any library / wrapper. The following commands are recognized by the Teensy:

* buzz frequency duration
* nobuzz
* led0 r g b
* led1 r g b

The device has three output devices: two individually controllable RGB LED-s (with adjustable brightness) and a Piezo buzzer for sound output.

# Libraries

Libraries, examples and tests are provided. All of them are written in Python (with some dependencies, like `pyserial`), although any other language that supports access to the serial interface can be used.

## Python Wrapper Object

The `Octo` class in `octo.py` wraps serial communication into a easy-to-use object with methods to control the output devices and read switch state transactions.

### CLI Usage

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

* octo.pdf - The schematic for the electronics, also [available at CircuitLab](https://www.circuitlab.com/circuit/s6dr46/octo/)
* teensy_octo.ino - C / Arduino compatible code for the firmware
* python/cli.py - A CLI wrapper for controlling the output devices on the Octo
* python/examples/ - Different examples on some use cases
* python/tests - Various test suites for testing the hardware (mostly during development)
* python/octo.py - A Python wrapper for controlling the output devices and reading button state changes

# Known bugs

* The buzz function causes LED1 to light as RED.
* Sending serial commands too fast (before the device responds with the ready message) won't work. Use `Octo.DELAY` constant between issuing serial commands.
* LED brightness can not be controlled with steps of 1 in the range of 0..255. About 5 seems to work.

# Hacking

* Follow [the guide](http://www.pjrc.com/teensy/loader.html) to customize the firmware
* Write your own `ButtonHandler` class(es)
* Use the observer pattern and write a plugin-based architecture that'd allow multiple modules to run simultaneously
* Re-write the firmware to allow for stand-alone applications, for example, running off of a battery pack

## Ideas

* A new Facebook message / Tweet / mail notifier
* Unittest runner with visual / audio feedback (PHPUnit)
* IO device for starting / stopping system (or remote) services
* HCI device for preprogrammed text input (can act as an external keyboard)
* Mood light
* Indicator for system load levels
* Digital authentication device for unlocking the screensaver
* Shortcut for userspace programs

# Afterword

Happy (late) birthday, [Waher](http://waher.net).
Licenced under the MIT licence.

## Authors

* [Ando Roots](http://sqroot.eu)
* [Valeria Gasik](http://sokeri.org)