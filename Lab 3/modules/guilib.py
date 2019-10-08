"""
Contains code for a GUIWindow object to prompt the user to select P,I, and D
values to use on the robot.
"""
import tkinter as tk

class GUIWindow:
    def __init__(self):
        self.root = tk.Tk()

        self.Plabel = tk.Label(self.root, text="P value:")
        self.Plabel.pack()
        self.Pbox = tk.Spinbox(self.root, from_=0, to=10, increment=0.1)
        self.Pbox.pack()

        self.Ilabel = tk.Label(self.root, text="I value:")
        self.Ilabel.pack()
        self.Ibox = tk.Spinbox(self.root, from_=0, to=10, increment=0.1)
        self.Ibox.pack()

        self.Dlabel = tk.Label(self.root, text="D value:")
        self.Dlabel.pack()
        self.Dbox = tk.Spinbox(self.root, from_=0, to=10, increment=0.1)
        self.Dbox.pack()

    def update(self):
        self.root.update() # Update the Tkinter window

        P = self.Pbox.get()
        I = self.Ibox.get()
        D = self.Dbox.get()
        return P,I,D

if __name__ == "__main__":
    w = GUIWindow()

    while True:
        print(w.update())
