"""
By: Amy Phung and Sophie Wu

Computes best fit line given a set of points and graphs output
"""

import matplotlib.pyplot as plt
import numpy as np
import csv

x = []
y = []

x_verify = []
y_verify = []

with open('Calibration2.csv','r') as csvfile:  # File containing calibration pts
    plots = csv.reader(csvfile, delimiter=',')
    for row in plots:
        x.append(int(row[0]))
        y.append(int(row[1]))

# Calculate Polynomial Best Fit
z = np.polyfit(y, x, 5)
f = np.poly1d(z)
print(f)

# Calulate best fit line
y_new = np.linspace(y[0], y[-1], 50)
x_new = f(y_new)

# Plot Verification Points
with open('Calibration1.csv','r') as csvfile: # File containing verification pts
    plots = csv.reader(csvfile, delimiter=',')
    for row in plots:
        x_verify.append(int(row[0]))
        y_verify.append(int(row[1]))


plt.plot(y,x,'b*',label='Calibration Data Points')
plt.plot(y_new, x_new, 'r-', label='Best Fit Line')
plt.plot(y_verify,x_verify,'go',label='Verification Data Points')
plt.xlabel('analogRead Output')
plt.ylabel('Distance from IR Sensor (in)')
plt.title('IR Calibration Curve')
plt.legend()
plt.show()
