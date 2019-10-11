import pandas as pd
import matplotlib.pyplot as plt

data = pd.read_csv("raw_data/raw_data2.csv")

print(data.head())

fig = plt.figure()

ax1 = fig.add_subplot(3,1,1)
ax2 = fig.add_subplot(3,1,2)
ax3 = fig.add_subplot(3,1,3)

ax1.title.set_text('Raw Sensor Output')
ax2.title.set_text('Normalized Sensor Output')
ax3.title.set_text('Speed Commands')

ax1.plot(data["Time"], data["LSensor"], label="Left")
ax1.plot(data["Time"], data["RSensor"], label="Right")
ax1.legend()
ax1.set_ylabel("analogRead() Output \n (maps 0-5V as 0-1023)")

ax2.plot(data["Time"], data["NLSensor"], label="Left")
ax2.plot(data["Time"], data["NRSensor"], label="Right")
ax2.legend()
ax2.set_ylabel("normalizeReading() Output \n (maps 0-1023 to 0-1000)")

ax3.plot(data["Time"], data["LSpeed"], label="Left")
ax3.plot(data["Time"], data["RSpeed"], label="Right")
ax3.legend()
ax3.set_ylabel("Speed PWM Command \n (between 0 and 255)")

plt.xlabel("Time (s)")

plt.show()
