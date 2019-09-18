import numpy as np
import matplotlib.pyplot as plt

class ColorPlot():
    """
    Object to plot scatterplots in realtime with z data represented as color.

    To use, initialize object, then call the update function with the newest
    data point
    """
    def __init__(self):
        # Set plot to interactive mode
        plt.ion()

        # Public Variables
        self.data = np.array([[0,0,0]])

        # Private Variables
        self._fig, self._ax = plt.subplots()

    def update(self, new_point):
        x, y, z = new_point

        self.data = np.append(self.data, np.array([new_point]), axis=0)

        scale = 200.0 # Make points bigger
        color = self._computeColor(z) # Map Z to color value
        self._ax.scatter(x, y, c=color, s=scale, label=color,
                   alpha=0.3, edgecolors='none')
        # self._ax.scatter(x,y,c=color, edgecolors='none')
        plt.draw()
        plt.pause(0.1)

    def _computeColor(self, z):
        """
        Remmaps Z value to a color on the blue-green spectrum based on the
        current min and max Z values

        Args:
            z (numerical): new point z value
        Returns:
            color (tuple): Tuple of size 3 with values between 0 and 1
                           representing RGB values
        """
        # Note: only scales color to new point, doesn't change old points
        min_z = float(np.min(self.data[:,2])) # Only look at z data
        max_z = float(np.max(self.data[:,2]))

        if (min_z == max_z):
            p = 0
        else:
            p = float((z-min_z) / (max_z - min_z))
        color = [0,1-p,p]

        return color

if __name__ == '__main__':
    # For testing
    plot = ColorPlot()
    for i in range(0,10):
        pt = (i,i,i)
        plot.update(pt)
    for i in [10,9,8,7,6,5,4,3,2,1]:
        pt = (i+1,i,i)
        plot.update(pt)
