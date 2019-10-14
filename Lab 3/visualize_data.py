import pandas as pd
import matplotlib.pyplot as plt

data = pd.read_csv("raw_data/raw_data2.csv")

print(data.head())

fig = plt.figure()

ax1 = fig.add_subplot(2,1,1)
ax3 = fig.add_subplot(2,1,2)

ax1.title.set_text('Raw Sensor Output and Motor Commands Over Time - Left Wheel')
ax2 = ax1.twinx()

ax1.plot(data["Time"], data["LSensor"], 'b-', label="Left Sensor")
ax2.plot(data["Time"], data["LSpeed"], 'g-', label="Left Speed")
ax1.legend()
ax2.legend()

ax1.set_ylabel("analogRead() Output \n (maps 0-5V as 0-1023)")
ax2.set_ylabel("Speed PWM Command \n (between 0 and 255)")

ax3.title.set_text('Raw Sensor Output and Motor Commands Over Time - Right Wheel')
ax4 = ax3.twinx()

ax3.plot(data["Time"], data["RSensor"], 'b-', label="Right Sensor")
ax4.plot(data["Time"], data["RSpeed"], 'g-', label="Right Speed")
ax3.legend()
ax4.legend()

ax3.set_ylabel("analogRead() Output \n (maps 0-5V as 0-1023)")
ax4.set_ylabel("Speed PWM Command \n (between 0 and 255)")

# For some reason all of these are necessary...
ax1.set_xlabel("Time (s)")
ax2.set_xlabel("Time (s)")
ax3.set_xlabel("Time (s)")
ax4.set_xlabel("Time (s)")
plt.xlabel("Time (s)")

plt.show()
