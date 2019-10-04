"""
By: Amy Phung

Contains code for a SerialConnection object to connect to the Arduino

Expected format:
v
P
I
D
"""

import serial

class SerialConnection:
    """
    Sets up a serial connection with an Arduino and sends a new PID value when
    needed.

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

        self.prev_PID1 = (0,0,0)
        self.PID1 = (0,0,0)
        self.prev_PID2 = (0,0,0)
        self.PID2 = (0,0,0)


        # print(self.PID[0])

    def update(self):
        # data = self.arduino.readline()
        # if data:
	    #       print(data.rstrip('\n')) #strip out the new lines for now
		# (better to do .read() in the long run for this reason

        if (self.PID1 != self.prev_PID1 or self.PID2 != self.prev_PID2): # If PID values have been changed, send them
            self.arduino.write("p")
            self.arduino.write(str(self.PID1[0]))#[:3]))
            self.arduino.write("i")
            self.arduino.write(str(self.PID1[1]))#[:3]))
            self.arduino.write("d")
            self.arduino.write(str(self.PID1[2]))#[:3]))
            self.arduino.write("e")

            self.prev_PID1 = self.PID1

            self.arduino.write("p")
            self.arduino.write(str(self.PID2[0]))#[:3]))
            self.arduino.write("i")
            self.arduino.write(str(self.PID2[1]))#[:3]))
            self.arduino.write("d")
            self.arduino.write(str(self.PID2[2]))#[:3]))
            self.arduino.write("e")

            self.prev_PID2 = self.PID2


if __name__ == "__main__":
    w = SerialConnection()

    while True:
        w.update()
        # tk.update()
