import serial

class Octo:

    BUTTON_PRESSED = 1
    BUTTON_RELEASED = 2


    def __init__(self, address):
        self.ser = serial.Serial(address, 9600)

    def buzz(self, freq, duration):
        self.ser.write("buzz %s %s" % (freq, duration))

    def led(self, r, g, b):
        self.ser.write("led %s %s %s" % (r, g, b))

    #def read_button(self, button_number):


    def __del__(self):
        self.ser.close()




octo = Octo('/dev/ttyACM0')
octo.buzz(1000, 40)
octo.led(100,50,240)