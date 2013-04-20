#!/usr/bin/env python
import subprocess
import time

from octo import Octo

octo = Octo('/dev/ttyACM0')

def main():
    octo.reset()
    while True:

        button_state = octo.read_button(1)
        if button_state is octo.BUTTON_RELEASED:
            print "Release"
            handle_button(1)
        elif button_state is octo.BUTTON_PRESSED:
            print "Press"

        time.sleep(0.3)

def handle_button(button_number):
    if button_number is 1:
        exit_code = subprocess.call("cd /work/areston/erebor; phpunit --configuration=build/phpunit.xml --group=app.api", shell=True)
        print "exited with code %s" % exit_code
        if exit_code is 0:
            octo.buzz(3500, 500)
            time.sleep(1)
            octo.led0(255,0,255)
        else:
            for i in range (0,3):
                octo.buzz(600,200)
                time.sleep(0.3)
            time.sleep(0.4)
            octo.led0(0,255,255)


if __name__ == '__main__':
    main()