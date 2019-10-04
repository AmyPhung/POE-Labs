"""
By: Amy Phung and Sophie Wu

Contains code for a SerialConnection object to connect to the Arduino

Expected format:
p
xdata
ydata
zdata
"""

import serial

class SerialConnection:
    """
    Sets up a serial connection with an Arduino and parses the input to record
    a new point every time the update function is called.

    Args:
        port (str): String containing Arduino port. Defaults to '/dev/ttyACM0'
        baud (int): Baud rate for connection. Defaults to 115200

    Attributes:
        self.arduino (Serial): Arduino connection
        self.new_point (tuple): New point from Arduino (inches, cartesian)
    """
    def __init__(self, port='/dev/ttyACM0', baud=115200):
        self.arduino = serial.Serial(port, baud, timeout=.1)
        self.new_point = None

    def update(self):
        data = self.arduino.readline()[:-2] # -2 to remove carraige return
        d = data.decode('ASCII')
        if d == 'p': # Begin reading new point
            x = float(self.arduino.readline()[:-2])
            y = float(self.arduino.readline()[:-2])
            z = float(self.arduino.readline()[:-2])

            self.new_point = (x,y,z)
        else:
            self.new_point = None
