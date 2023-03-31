import tkinter as tk
import threading
from numpy import *
from tkinter.colorchooser import askcolor
import copy

n = 100
zoom_index = 6
zoom = [10, 14, 20, 25, 28, 35, 40, 50, 56, 70, 100, 140, 175, 200]

cero_grid = []
new_grid = []
next_gen = []

color_vivo = '#ffffff'
color_muerto = '#000000'

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


def update_canvas() :

    global new_grid
    global color_vivo
    global color_muerto
    global canvas
    global zoom_index

    # Si se hace zoom, signigica que queremos que las celdas tengan mayor tamaño
    # por lo que hay que avanzar el indice de zoom a la derecha

    canvas_w = canvas.winfo_width()
    canvas_h = canvas.winfo_height()
    canvas.configure(bg=color_muerto)
    canvas.delete("rect")

    cell_w = zoom[zoom_index]
    cell_h = zoom[zoom_index]

    for i in range(n):
        for j in range(n):
            x0 = j * cell_w
            y0 = i * cell_h
            x1 = (j + 1) * cell_w
            y1 = (i + 1) * cell_h
            if new_grid[i][j] == True:
                canvas.create_rectangle(x0, y0, x1, y1, fill=color_vivo, tags="rect")

def canvas_click(event):
    print("Mouse clicked at", event.x, event.y)

def next_step() :

    global canvas
    global next_gen
    global new_grid
    global cero_grid

    calculate_next_gen(0, n - 1, 0, n - 1, 0 , False)

    new_grid = copy.deepcopy(next_gen)
    next_gen = copy.deepcopy(cero_grid)

    update_canvas()

def cambiar_zoom(opcion) :

    global zoom_index

    if opcion == 1 :
        zoom_index = zoom_index + 1 if zoom_index + 1 < 14 else zoom_index
    else :
        zoom_index = zoom_index - 1 if zoom_index - 1 >= 0 else 0

    return 

def seleccionador_color(opcion):
    colors = askcolor(title="Selecciona el color")

    if opcion  == 1 :
        global color_muerto 
        color_muerto = colors[1]
        seleccionar_color()
        return
    else :
        global color_vivo 
        color_vivo = colors[1]
        seleccionar_color()
        return

def seleccionar_color():

    global canvas

    # create the pop-up window
    popup = tk.Toplevel(root)
    popup.geometry("650x250")

    popup.columnconfigure(1, weight=2)
    popup.columnconfigure(3, weight=2)

    popup.rowconfigure(0, weight=0)

    # create the first button with a label
    button1 = tk.Button(popup, width=20, height=3, text="", bg=color_muerto, activebackground=color_muerto, command=lambda:(popup.destroy(), seleccionador_color(1)))
    label1 = tk.Label(popup, width=20, height=3, text="Celdas Muertas")

    # create the second button with a label
    button2 = tk.Button(popup, width=20, height=3, text="", bg=color_vivo, activebackground=color_vivo, command=lambda:(popup.destroy(), seleccionador_color(2)))
    label2 = tk.Label(popup,  width=20, height=3, text="Celdas vivas")

    button3 = tk.Button(popup, width=10, height=3, text="OK", command = lambda:(popup.destroy(), update_canvas()))

    button1.grid(row=1, column=1, padx=(10,10), pady=(50,0))
    label1.grid(row=2, column=1, padx=(10,10), pady=(0,0))
    
    button2.grid(row=1, column=3, padx=(10,10), pady=(50,0))
    label2.grid(row=2, column=3, padx=(10,10), pady=(0,0))


    button3.grid(row=3, column=2, padx=(10,10), pady=(0,0))

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

    update_canvas()


# create the main window
root = tk.Tk()
root.geometry("1650x860")
root.configure(bg="#134f5c")
root.title("Conway's Game of Life")
root.after(50, on_load)

# create the canvas on the right side
canvas = tk.Canvas(root, width=1400, height=1400, bg=color_muerto)
canvas.pack(side=tk.RIGHT, padx=10, pady=(70,70))

# create a frame to hold the buttons
button_frame = tk.Frame(root, bg="#134f5c")
button_frame.pack(side=tk.TOP, padx=25, pady=10)

# create a few buttons in the button frame
button1 = tk.Button(button_frame, width=20, height=3, text="Evolucion automatica", command=lambda:update_canvas())
button1.pack(side=tk.TOP, pady=15, expand=True)

button2 = tk.Button(button_frame, width=20, height=3, text="Siguiente evolucion", command=lambda:next_step())
button2.pack(pady=15, expand=True)

button3 = tk.Button(button_frame, width=20, height=3, text="Detener")
button3.pack(pady=15, expand=True)

# create a frame to hold the buttons
button_frame_zoom = tk.Frame(root, bg="#134f5c")
button_frame_zoom.pack(pady=15)

button_zoom_1 = tk.Button(button_frame_zoom, width=7, height=2, text="+", command=lambda:cambiar_zoom(1))
button_zoom_2 = tk.Button(button_frame_zoom, width=7, height=2, text="-", command=lambda:cambiar_zoom(2))

button_zoom_1.pack(pady=7, expand=True)
button_zoom_2.pack(pady=7, expand=True)


button_color = tk.Button(button_frame, width=20, height=3, text="Seleccionar Color", command=lambda:seleccionar_color())
button_color.pack(pady=(100,15), expand=True)


# create a frame to hold the buttons
button_frame2 = tk.Frame(root, bg="#134f5c")
button_frame2.pack(side=tk.BOTTOM)

boton_guardar = tk.Button(button_frame2, width=10, height=3, text="Guardar")
boton_abrir = tk.Button(button_frame2, width=10, height=3, text="Abrir")

boton_guardar.pack(pady=7, expand=True)
boton_abrir.pack(pady=7, expand=True)


label_test = tk.Label(root, width=20, height=3, text="Test", bg='#ffffff')
label_test.place(x=600, y = 798)


# bind the <Button-1> event to the canvas_click function
canvas.bind("<Button-1>", canvas_click)

# run the main loop
root.mainloop()