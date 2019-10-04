from modules.guilib import GUIWindow
from modules.seriallib import SerialConnection

sc = SerialConnection(port='/dev/ttyACM1')
gui = GUIWindow()

while True:
    PIDvals = gui.update()
    print(PIDvals)
    sc.PID1 = PIDvals[:3]
    sc.PID2 = PIDvals[3:]
    sc.update()
