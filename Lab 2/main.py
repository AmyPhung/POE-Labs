"""
By: Amy Phung and Sophie Wu

Gets a cartesian point from an Arduino over serial and displays it in a 2D graph
where depth is represented by color
"""

from modules.plotlib import ColorPlot
from modules.seriallib import SerialConnection
from modules.datalib import DataSaver

arduino = SerialConnection('/dev/ttyACM0', 115200)
plot = ColorPlot()
ds = DataSaver()

while True:
	arduino.update()

	if arduino.new_point:
		plot.update(arduino.new_point)
		ds.update(arduino.new_point)
		print("new point")
