"""
By: Amy Phung and Sophie Wu

Gets a cartesian point from an Arduino over serial and displays it in a 2D graph
where depth is represented by color
"""

from modules.plotlib import ColorPlot
from modules.seriallib import SerialConnection

arduino = SerialConnection('/dev/ttyACM0', 115200)
plot = ColorPlot()

while True:
	arduino.update()
	if arduino.new_point:
		plot.update(arduino.new_point)
