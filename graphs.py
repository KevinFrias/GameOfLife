import tkinter as tk
import math
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
from matplotlib.figure import Figure

def create_graph(ax, canvas, option):
    # Create a new subplot
    ax.clear()
    
    label_Y = ""
    nombre_archivo = ""

    if option == "Population" :
        label_Y = "# Population"
    else :
        label_Y = "# Entropy"

    if option == "Entropy" :
        nombre_archivo = "entriopia.txt"
    else :
        nombre_archivo = "normal.txt"

    # Create the x and y axis labels
    ax.set_xlabel("Iteration")
    ax.set_ylabel(label_Y)

    index = 0
    data = []

    # Open the file for reading
    with open(nombre_archivo, "r") as file:
        # Loop over each line in the file
        for line in file:
            line = float(line.strip())

            if option == "Population Log10" and line > 0:
                line = math.log10(line)

            data.append(tuple([index, line]))
            index += 1

    x, y = zip(*data) # unpack the data into separate x and y tuples

    ax.scatter(x, y, color="blue")
    canvas.draw()

def main():
    root = tk.Tk()

    # Set the window size
    root.geometry("1250x700")
    # Create a label to display the selection
    label = tk.Label(root, text="Population", font=("Arial", 18))
    label.pack(pady=(5,5))

    # Create the figure and graph
    fig = Figure(figsize=(11, 6))
    ax = fig.add_subplot(111)

    # Embed the figure in a Tkinter window
    canvas = FigureCanvasTkAgg(fig, master=root)
    canvas.draw()
    canvas.get_tk_widget().pack()


    create_graph(ax, canvas,  "Population")

    # Create a variable to store the selected option
    var = tk.StringVar(root)
    var.set("Population")

    # Create the drop-down menu
    option_menu = tk.OptionMenu(root, var, "Population", "Population Log10", "Entropy", command=lambda choice: create_graph(ax, canvas, choice))
    option_menu.config(width=15, heigh=3, font=("Arial", 14))
    option_menu.pack(pady=(20,0))

    root.mainloop()

if __name__ == "__main__":
    main()
