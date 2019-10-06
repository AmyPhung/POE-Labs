import tkinter as tk

class GUIWindow:
    def __init__(self):
        self.root = tk.Tk()

        self.P1label = tk.Label(self.root, text="P1 value:")
        self.P1label.pack()
        self.P1box = tk.Spinbox(self.root, from_=0, to=10, increment=0.1)
        self.P1box.pack()

        self.I1label = tk.Label(self.root, text="I1 value:")
        self.I1label.pack()
        self.I1box = tk.Spinbox(self.root, from_=0, to=10, increment=0.1)
        self.I1box.pack()

        self.D1label = tk.Label(self.root, text="D1 value:")
        self.D1label.pack()
        self.D1box = tk.Spinbox(self.root, from_=0, to=10, increment=0.1)
        self.D1box.pack()

        self.P2label = tk.Label(self.root, text="P2 value:")
        self.P2label.pack()
        self.P2box = tk.Spinbox(self.root, from_=20, to=75, increment=1)
        self.P2box.pack()

        self.I2label = tk.Label(self.root, text="I2 value:")
        self.I2label.pack()
        self.I2box = tk.Spinbox(self.root, from_=0, to=10, increment=0.1)
        self.I2box.pack()

        self.D2label = tk.Label(self.root, text="D2 value:")
        self.D2label.pack()
        self.D2box = tk.Spinbox(self.root, from_=0, to=10, increment=0.1)
        self.D2box.pack()

    def update(self):
        self.root.update()

        P1 = self.P1box.get()
        I1 = self.I1box.get()
        D1 = self.D1box.get()
        P2 = self.P2box.get()
        I2 = self.I2box.get()
        D2 = self.D2box.get()
        return P1,I1,D1,P2,I2,D2

if __name__ == "__main__":
    w = GUIWindow()

    while True:
        print(w.update())
        # tk.update()
