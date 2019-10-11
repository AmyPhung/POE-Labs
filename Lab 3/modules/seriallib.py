"""
Contains code for a SerialConnection object to connect to the Arduino
"""

import serial
import keyboard

class SerialConnection:
    """
    Sets up a serial connection with an Arduino and sends a new PID value when
    needed. Toggles between a running and an e-stopped state each time the
    spacebar is pressed.

    Args:
        port (str): String containing Arduino port. Defaults to '/dev/ttyACM0'
        baud (int): Baud rate for connection. Defaults to 115200

    Attributes:
        self.arduino (Serial): Arduino connection
        self.prev_PID (tuple): Tuple containing previous (P,I,D) values
        self.PID (tuple): Tuple containing (P,I,D) values
    """
    def __init__(self, port='/dev/ttyACM0', baud=115200):
        self.arduino = serial.Serial(port, baud, timeout=.1)

        self.prev_PID = (0,0,0)
        self.PID = (0,0,0)

    def update(self):
        if keyboard.is_pressed(' '):
            self.arduino.write("e") # send e to estop arduino
        if (self.PID != self.prev_PID): # If PID values have been changed, send them
            self.arduino.write("a") # disposable character - not an e
            self.arduino.write("p")
            self.arduino.write(str(self.PID[0]))#[:3]))
            self.arduino.write("i")
            self.arduino.write(str(self.PID[1]))#[:3]))
            self.arduino.write("d")
            self.arduino.write(str(self.PID[2]))#[:3]))
            self.arduino.write("e")

            self.prev_PID = self.PID

if __name__ == "__main__":
    w = SerialConnection()

    while True:
        w.update()
