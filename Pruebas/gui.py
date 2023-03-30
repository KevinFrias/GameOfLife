import tkinter as tk
import threading
from numpy import *
import copy

n = 8
cero_grid = []
new_grid = []
next_gen = []



def update_canvas(canvas) :
    canvas_w = canvas.winfo_width()
    canvas_h = canvas.winfo_height()
    canvas.delete("rect")

    global new_grid

    cell_w = canvas_w // len(new_grid[0])
    cell_h = canvas_h // len(new_grid)

    for i in range(n):
        for j in range(n):
            x0 = j * cell_w
            y0 = i * cell_h
            x1 = (j + 1) * cell_w
            y1 = (i + 1) * cell_h
            if new_grid[i][j] == True:
                canvas.create_rectangle(x0, y0, x1, y1, fill="white", tags="rect")

def calculate_next_gen(x1, x2, y1, y2, iteracion, nulo):
    if iteracion == 0 :

        global new_grid
        global next_gen

        for i in range(x1, x2+1):
            for j in range(y1, y2+1):

                poblacion = 0

                if nulo == True:
                    if i - 1 >= 0 :
                        if j-1 >=0 and new_grid[j-1][i-1] == True :
                            poblacion = poblacion + 1

                        if new_grid[j][i-1] == True :
                            poblacion = poblacion + 1

                        if j+1 < n and new_grid[j+1][i-1] == True:
                            poblacion = poblacion + 1
                

                    if j - 1 >= 0 and new_grid[j-1][i] == True:
                        poblacion = poblacion + 1

                    if j + 1 < n and new_grid[j+1][i] == True:
                        poblacion = poblacion + 1


                    if i + 1 < n :
                        if j-1 >= 0 and new_grid[j-1][i+1] == True:
                            poblacion = poblacion + 1

                        if new_grid[j][i+1] == True :
                            poblacion = poblacion + 1

                        if j+1 < n and new_grid[j+1][i+1] == True :
                            poblacion = poblacion + 1
                else :
                    indexIzquierda = n - 1 if i - 1 < 0 else i - 1
                    indexDerecha = 0 if i + 1 >= n else i + 1

                    indexArriba = n - 1 if j - 1 < 0 else j - 1
                    indexAbajo = 0 if j + 1 >= n else j + 1

                    poblacion = poblacion + 1 if new_grid[indexArriba][indexIzquierda] == True else poblacion
                    poblacion = poblacion + 1 if new_grid[j][indexIzquierda] == True else poblacion
                    poblacion = poblacion + 1 if new_grid[indexAbajo][indexIzquierda] == True else poblacion
                    
                    poblacion = poblacion + 1 if new_grid[indexArriba][i] == True else poblacion
                    poblacion = poblacion + 1 if new_grid[indexAbajo][i] == True else poblacion
                    
                    poblacion = poblacion + 1 if new_grid[indexArriba][indexDerecha] == True else poblacion
                    poblacion = poblacion + 1 if new_grid[j][indexDerecha] == True else poblacion
                    poblacion = poblacion + 1 if new_grid[indexAbajo][indexDerecha] == True else poblacion


                if new_grid[j][i] == True :
                    if poblacion >= 4 or poblacion <= 1 :
                        next_gen[j][i] = False
                    else :
                        next_gen[j][i] = True
                else :
                    if poblacion == 3 :
                        next_gen[j][i] = True

                # Aqui es donde dibujo la celda en la pantalla del juego

        return

    mitad_x = x2 // 2
    mitad_y = y2 // 2

    cuadrantes = [None] * 4

    cuadrantes[0] = threading.Thread(target=calculate_next_gen, args=(x1, mitad_x, y1, mitad_y, iteracion + 1, nulo))
    cuadrantes[1] = threading.Thread(target=calculate_next_gen, args=(mitad_x + 1, x2, y1, mitad_y, iteracion + 1, nulo))
    cuadrantes[2] = threading.Thread(target=calculate_next_gen, args=(x1, mitad_x, mitad_y + 1, y2, iteracion + 1, nulo))
    cuadrantes[3] = threading.Thread(target=calculate_next_gen, args=(mitad_x + 1, x2, mitad_y + 1, y2, iteracion + 1, nulo))

    for i in range(4):
        cuadrantes[i].start()

    for i in range(4):
        cuadrantes[i].join()

def canvas_click(event):
    print("Mouse clicked at", event.x, event.y)

def next_step(canvas) :
    calculate_next_gen(0, n - 1, 0, n - 1, 0 , False)
    update_canvas(canvas)

    global next_gen
    global new_grid
    global cero_grid

    new_grid = copy.deepcopy(next_gen)
    next_gen = copy.deepcopy(cero_grid)


def on_load() :
    global new_grid
    global next_gen
    global cero_grid

    cero_grid = [[False for j in range(n)] for i in range(n)]
    new_grid = copy.deepcopy(cero_grid)
    next_gen = copy.deepcopy(cero_grid)

    new_grid[0][1] = True
    new_grid[1][2] = True
    new_grid[2][0] = True
    new_grid[2][1] = True
    new_grid[2][2] = True

    print(new_grid)


# create the main window
root = tk.Tk()
root.geometry("1650x860")
root.configure(bg="#134f5c")
root.title("Conway's Game of Life")

# create the canvas on the right side
canvas = tk.Canvas(root, width=1400, height=1400, bg="black")
canvas.pack(side=tk.RIGHT, padx=10, pady=10)

# create a frame to hold the buttons
button_frame = tk.Frame(root, bg="#134f5c")
button_frame.pack(side=tk.LEFT, padx=25, pady=10)

# create a few buttons in the button frame
button1 = tk.Button(button_frame, width=20, height=3, text="Button 1", command=lambda:update_canvas(canvas))
button1.pack(side=tk.TOP, pady=15, expand=True)

button2 = tk.Button(button_frame, width=20, height=3, text="Button 2", command=lambda:next_step(canvas))
button2.pack(pady=15, expand=True)

button3 = tk.Button(button_frame, width=20, height=3, text="Button 3")
button3.pack(pady=15, expand=True)

# bind the <Button-1> event to the canvas_click function
canvas.bind("<Button-1>", canvas_click)

root.after(0, on_load)

# run the main loop
root.mainloop()