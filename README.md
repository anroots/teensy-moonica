# Teensy Octo

A USB IO device for developers. Packaged as a cute eight-legged creature, this device connects to your USB port to do your bidding. Press any of the eight legs to activate pre-programmed commands or write software to one of its three output devices as an indicator of interesting events in the virtual world.

The hardware and software are open to hacking. The user has to program the macros for each button himself.

## Hardware Specs

* Teensy Octo 2.0 microcontroller
* 8 pushbuttons
* 2 individually controller PWM RGB LEDs
* A Piezo Buzzer

## External Input Device

The computer recognizes the Teensy as a gamepad/joystic type of device. By default it's programmed to send joystick button press events on each button press. Open the gamepad config window from your control panel to see the mapping.

## Serial Communication

The device communicates with the computer over serial. Information is sent in both direction:

* Octo -> PC: Button press / button release events
* PC -> Octo: LED / Buzzer control

### Initialization message

```
Teensy Octo; booted in 200ms - which is amazingly fast.


=== Available serial commands: ===

buzz frequency duration
nobuzz
led0 r g b
led1 r g b

More info: http://github.com/anroots/teensy-octo

Waiting for button presses or serial commands.
```

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
* python/example.py - An example implementation of using the `Octo` library. Contains a `ButtonHandler` that reacts to button presses.
* python/test.py - A test suite for the output devices on the Octo
* python/octo.py - A Python wrapper for controlling the output devices and reading button state changes

# Known bugs

* The buzz function causes LED1 to light as RED.
* Sending serial commands too fast (before the device responds with the ready message) won't work
* LED brightness can not be controlled with steps of 1 in the range of 0..255

# Hacking

* Follow [the guide](http://www.pjrc.com/teensy/loader.html) to customize the firmware
* Write your own `ButtonHandler` class(es)
* Use the observer pattern and write a plugin-based architecture that'd allow multiple modules to run simultaneously

## Ideas

* A new Facebook message / Tweet / mail notifier
* Unittest runner with visual / audio feedback (PHPUnit)
* IO device for starting / stopping system (or remote) services
* HCI device for preprogrammed text input (can act as an external keyboard)
* Mood light
* Indicator for system load levels
* Digital authentication device for unlocking the screensaver
* Shortcut for userspace programs

# Authors

* [Ando Roots](http://sqroot.eu)
* [Valeria Gasik](http://sokeri.org)