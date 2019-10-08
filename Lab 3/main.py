"""
Creates a GUI and serial connection to arduino to regularly update PID values
"""

from modules.guilib import GUIWindow
from modules.seriallib import SerialConnection

sc = SerialConnection(port='/dev/ttyACM0')
gui = GUIWindow()

while True:
    PIDvals = gui.update()
    sc.PID = PIDvals
    sc.update()
