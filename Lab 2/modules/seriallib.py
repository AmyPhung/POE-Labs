import serial

class SerialConnection:
    def __init__(self, port, baud):
        self.arduino = serial.Serial(port, baud, timeout=.1)
        self.raw_data = 0
        self.new_point = None

    def update(self):
        data = self.arduino.readline()[:-2] # -2 to remove carraige return
        if data:
            self.raw_data = data
            self.new_point = parseInput(new_data)
        else:
            new_point = None

    def parseInput(self, data):
        # TODO: Fill this out depeding on input
        point = (0,0,0)
        return point
