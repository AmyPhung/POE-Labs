import serial

# For testing
import random

class SerialConnection:
    def __init__(self, port, baud):
        self.arduino = serial.Serial(port, baud, timeout=.1)
        self.new_point = None

    def update(self):
        data = self.arduino.readline()[:-2] # -2 to remove carraige return
        if data == 'p': # Begin new point
            x = float(self.arduino.readline()[:-2])
            y = float(self.arduino.readline()[:-2])
            z = float(self.arduino.readline()[:-2])

            self.new_point = (x,y,z)
        else:
            self.new_point = None

        print(self.new_point)
