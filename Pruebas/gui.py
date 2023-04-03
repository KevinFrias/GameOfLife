import tkinter as tk
import threading
from numpy import *
from tkinter.colorchooser import askcolor
from tkinter import filedialog
import copy
import pickle

n = 100
zoom_index = 11
zoom = [7, 10, 14, 20, 25, 28, 35, 50, 70, 100, 140, 175, 350, 700]
automatico = False
movimiento = 1

cero_grid = []
new_grid = []
next_gen = []

color_vivo = '#ffffff'
color_muerto = '#000000'
color_fondo= '#134f5c'

mutex = threading.Lock()

def calculate_next_gen(x1, x2, y1, y2, iteracion, nulo):
    if iteracion == 0 :
        global canvas
        global new_grid
        global next_gen

        canvas.configure(bg=color_muerto)
        canvas.delete("rect")


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

                cell_w = zoom[zoom_index]
                cell_h = zoom[zoom_index]
                a1 = j * cell_w
                b1 = i * cell_h
                a2 = (j + 1) * cell_w
                b2 = (i + 1) * cell_h

                origen = 1249

                if new_grid[j][i] == True :
                    if poblacion >= 4 or poblacion <= 1 :
                        next_gen[j][i] = False
                    else :
                        next_gen[j][i] = True
                        canvas.create_rectangle(a1,b1, a2, b2, fill=color_vivo, tags="rect")

                else :
                    if poblacion == 3 :
                        next_gen[j][i] = True
                        canvas.create_rectangle(a1,b1, a2, b2, fill=color_vivo, tags="rect")

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

def limpiar_juego():
    global canvas
    canvas.configure(bg=color_muerto)
    canvas.delete("rect")





def update_canvas(x1, x2, y1, y2, iteracion, mutex) :
    if iteracion == 1 :
        global color_vivo
        global canvas
        global zoom_index

        cell_w = zoom[zoom_index]
        cell_h = zoom[zoom_index]

        for i in range(x1, x2+1):
            for j in range(y1, y2+1):
                a0 = j * cell_w
                b0 = i * cell_h
                a1 = (j + 1) * cell_w
                b1 = (i + 1) * cell_h

                if new_grid[i][j] == True:
                    with mutex :
                        canvas.create_rectangle(a0, b0, a1, b1, fill=color_vivo, tags="rect")
                        print(a0, ',', b0, ' | ', a1, ',', b1)
        
        print()
                    
        return 

    mitad_x = x2 // 2
    mitad_y = y2 // 2

    cuadrantes = [None] * 4

    cuadrantes[0] = threading.Thread(target=update_canvas, args=(x1, mitad_x, y1, mitad_y, iteracion + 1, mutex))
    cuadrantes[1] = threading.Thread(target=update_canvas, args=(mitad_x + 1, x2, y1, mitad_y, iteracion + 1, mutex))
    cuadrantes[2] = threading.Thread(target=update_canvas, args=(x1, mitad_x, mitad_y + 1, y2, iteracion + 1, mutex))
    cuadrantes[3] = threading.Thread(target=update_canvas, args=(mitad_x + 1, x2, mitad_y + 1, y2, iteracion + 1, mutex))

    for i in range(4):
        cuadrantes[i].start()

    # Podemos quitar el join de los diferentes hilos, ya que con el uso de un mutex, aseguramos que unicamente se acceda 
    # al recurso compartido, en este caso, el canvas uno por hilo

    return 






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

def cambiar_zoom(opcion) :

    global zoom_index
    global automatico

    if opcion == 1 :
        zoom_index = zoom_index + 1 if zoom_index + 1 < len(zoom) else zoom_index
    else :
        zoom_index = zoom_index - 1 if zoom_index - 1 >= 0 else 0

    if automatico == False :
        limpiar_juego()
        update_canvas(0, n - 1, 0, n - 1, 0, mutex)

    return 


def cambiar_scroll(opcion):
    global movimiento
    global label_scroll_actual

    if opcion == 1 :
        movimiento = movimiento + 1 if n - movimiento + 1 >= 0 else movimiento
    else :
        movimiento = movimiento - 1 if movimiento - 1 > 0 else 1

    label_scroll_actual.configure(text=str(movimiento))

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

    button3 = tk.Button(popup, width=10, height=3, text="OK", command = lambda:(popup.destroy(), limpiar_juego(), update_canvas(0, n - 1, 0, n - 1, 0, mutex)))

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

    limpiar_juego()
    update_canvas(0, n - 1, 0, n - 1, 0, mutex)

def manejar_archivo(opcion):
    global new_grid

    if opcion == 1 :
        filename = filedialog.asksaveasfilename(defaultextension=".txt")
        if len(filename) != 0 :
            with open(filename, 'wb') as f:
                pickle.dump(new_grid, f)
    else :
        filename = filedialog.askopenfilename(defaultextension=".txt")
        with open(filename, 'rb') as f:
            new_grid = pickle.load(f)

        update_canvas(0, n - 1, 0, n - 1, 0, mutex)
    return 

def evolucion_automatica(opcion) :

    global automatico

    if opcion == 1 :
        automatico = True
    elif opcion == 2 :
        automatico = False
    elif opcion == 3:
        automatico = False
        next_step()

    return

def siguiente_paso():
    global automatico

    if automatico == True:
        next_step()

    root.after(100, siguiente_paso)

    return 


# Creamos la ventana principal con un tamaño determinado, un color de fondo, titulo que no cambie de tamaño y que muestre el primer estado en el juego
root = tk.Tk()
root.geometry("1650x880")
root.configure(bg=color_fondo)
root.title("Conway's Game of Life")
root.resizable(False, False)
root.after(50, on_load)


# Decimos que despues de cierto intervalo de tiempo, queremos checar si es que esta activa la opcion de evolucion automatica
root.after(100, siguiente_paso)


# Creamos el canvas y lo posicionamos en la parte derecha
canvas = tk.Canvas(root, width=1398, height=698, bg=color_muerto)
canvas.pack(side=tk.RIGHT, padx=10, pady=1)

# Creamos un marco para todos los botones con las diferentes opciones
button_frame = tk.Frame(root, bg=color_fondo)
button_frame.pack(side=tk.TOP, padx=25, pady=10)

# Creamos los botones para la evolucion automatica, para deter esta y para avanzar un unico paso en la evolucion
button1 = tk.Button(button_frame, width=20, height=3, text="Evolucion automatica", command=lambda:evolucion_automatica(1))
button1.pack(side=tk.TOP, pady=15, expand=True)

button2 = tk.Button(button_frame, width=20, height=3, text="Detener", command=lambda:evolucion_automatica(2))
button2.pack(pady=15, expand=True)

button3 = tk.Button(button_frame, width=20, height=3, text="Siguiente evolucion", command=lambda:evolucion_automatica(3))
button3.pack(pady=15, expand=True)





# Creamos un recuadro para poder guardar los botones de zoom
button_frame_zoom = tk.Frame(button_frame, bg=color_fondo)
button_frame_zoom.pack(pady=15)

# Creamos los botones de zoom
button_zoom_1 = tk.Button(button_frame_zoom, width=5, height=2, text="+", command=lambda:cambiar_zoom(1))
button_zoom_2 = tk.Button(button_frame_zoom, width=5, height=2, text="-", command=lambda:cambiar_zoom(2))

# Los posicionamos para que esten a un lado del otro
button_zoom_1.pack(side=tk.RIGHT, pady=4, padx=10, expand=True)
button_zoom_2.pack(side=tk.LEFT, pady=4, padx=10, expand=True)





# Creamos el boton para poder cambiar el color de las celdas vivas y muertas
button_color = tk.Button(button_frame, width=15, height=3, text="Seleccionar Color", command=lambda:seleccionar_color())
button_color.pack(pady=50, expand=True)






# Creamos un marco para poder posicionar correctamente los botones para guardar y abrir una configuracion establecida del juego
button_frame3 = tk.Frame(button_frame, bg=color_fondo)
button_frame3.pack(side=tk.TOP, pady=15, expand=True)

boton_arriba = tk.Button(button_frame3, width=5, height=1, text="^")
boton_abajo = tk.Button(button_frame3, width=5, height=1, text="v")
boton_derecha = tk.Button(button_frame3, width=5, height=1, text=">")
boton_izquierda = tk.Button(button_frame3, width=5, height=1, text="<")

boton_arriba.pack(side=tk.TOP, pady=1, expand=True)
boton_abajo.pack(side=tk.BOTTOM, pady=1, expand=True)
boton_derecha.pack(side=tk.RIGHT, pady=1, expand=True)
boton_izquierda.pack(side=tk.LEFT, pady=1, expand=True)




# Creamos un recuadro para poder guardar los botones de zoom
button_frame_scroll = tk.Frame(button_frame, bg=color_fondo)
button_frame_scroll.pack(pady=15)

# Los posicionamos para que esten a un lado del otro
button_scroll_2 = tk.Button(button_frame_scroll, width=3, height=2, text="-", command=lambda:cambiar_scroll(2))
button_scroll_2.pack(side="left")

# Creamos una etiqueta para mostrar la informacion relevante y la posicionamos dentro de la ventana principal
label_scroll_actual = tk.Label(button_frame_scroll, width=5, height=2, text=str(movimiento), bg='#999999')
label_scroll_actual.pack(padx=15, side="left")

# Creamos los botones de zoom
button_scroll_1 = tk.Button(button_frame_scroll, width=3, height=2, text="+", command=lambda:cambiar_scroll(1))
button_scroll_1.pack(side="left")






# Creamos un marco para poder posicionar correctamente los botones para guardar y abrir una configuracion establecida del juego
button_frame2 = tk.Frame(button_frame, bg=color_fondo)
button_frame2.pack(pady=15)

boton_guardar = tk.Button(button_frame2, width=5, height=3, text="Guardar", command=lambda:manejar_archivo(1))
boton_guardar.pack(side="left", pady=7, padx=5, expand=True)


boton_abrir = tk.Button(button_frame2, width=5, height=3, text="Abrir", command=lambda:manejar_archivo(2))
boton_abrir.pack(side="right", pady=7, padx=5, expand=True)








# Creamos una etiqueta para mostrar la informacion relevante y la posicionamos dentro de la ventana principal
label_test = tk.Label(root, width=20, height=3, text="Test", bg='#ffffff')
label_test.place(x=600, y = 798)


# bind the <Button-1> event to the canvas_click function
canvas.bind("<Button-1>", canvas_click)

# run the main loop
root.mainloop()