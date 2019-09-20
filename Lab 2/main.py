from modules.plotlib import ColorPlot
from modules.seriallib import SerialConnection

arduino = SerialConnection('/dev/ttyACM1', 115200)
plot = ColorPlot()

while True:
	arduino.update()
	if arduino.new_point:
		plot.update(arduino.new_point)
