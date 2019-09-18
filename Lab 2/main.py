from modules import seriallib
from modules import plotlib

arduino = seriallib.SerialConnection('/dev/ttyACM1', 115200)
plot = plotlib.ColorPlot()

while True:
	arduino.update()
	if arduino.new_point:
		plot.update(arduino.new_point)
