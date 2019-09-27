"""
By: Amy Phung and Sophie Wu

Computes best fit line given a set of points and graphs output
"""

import matplotlib.pyplot as plt
import numpy as np
import csv

x = []
y = []

with open('CAlibration.csv','r') as csvfile:
    plots = csv.reader(csvfile, delimiter=',')
    for row in plots:
        x.append(int(row[0]))
        y.append(int(row[1]))

# Calculate Polynomial Best Fit
z = np.polyfit(y, x, 5)
f = np.poly1d(z)
print(f)

# Calulate best fit line
x_new = np.linspace(x[0], x[-1], 50)
y_new = f(x_new)

plt.plot(x,y,'b*',label='Data Points')
plt.plot(x_new, y_new, 'r-', label='Best Fit Line')
plt.xlabel('Distance from IR Sensor (in)')
plt.ylabel('analogRead Output')
plt.title('IR Calibration Curve')
plt.legend()
plt.show()
