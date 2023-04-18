import tkinter as tk
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
from matplotlib.figure import Figure

def create_graph(fig, option):
    # Create a new subplot
    ax = fig.add_subplot(111)
    
    label_Y = ""
    nombre_archivo = ""

    if option == "Population" or "Population Log10":
        label_Y = "# Population"
    else :
        label_Y = "# Entropy"

    if option == "Population" :
        nombre_archivo = "normal.txt"
    elif option == "Entropy" :
        nombre_archivo = "entriopia.txt"
    else :
        nombre_archivo = "logaritmo.txt"

    # Create the x and y axis labels
    ax.set_xlabel("Iteration")
    ax.set_ylabel(label_Y)

    index = 0
    data = []

    # Open the file for reading
    with open(nombre_archivo, "r") as file:
        # Loop over each line in the file
        for line in file:
            # Strip whitespace from the beginning and end of the line
            line = line.strip()
            line = int(line)
            data.append(tuple([index, line]))
            index += 1

    print(data)

    x, y = zip(*data) # unpack the data into separate x and y tuples
    ax.scatter(x, y, color="blue")

def main():
    root = tk.Tk()

    # Set the window size
    root.geometry("1250x700")
    # Create a label to display the selection
    label = tk.Label(root, text="Population", font=("Arial", 18))
    label.pack(pady=(5,5))

    # Create the figure and graph
    fig = Figure(figsize=(11, 6))
    create_graph(fig, "Population")

    # Embed the figure in a Tkinter window
    canvas = FigureCanvasTkAgg(fig, master=root)
    canvas.draw()
    canvas.get_tk_widget().pack()


    # Create a variable to store the selected option
    var = tk.StringVar(root)
    var.set("Population")

    # Create the drop-down menu
    option_menu = tk.OptionMenu(root, var, "Population", "Population Log10", "Entropy", command=lambda choice: create_graph(fig, choice))
    option_menu.config(width=15, heigh=3, font=("Arial", 14))
    option_menu.pack(pady=(20,0))

    root.mainloop()

if __name__ == "__main__":
    main()
