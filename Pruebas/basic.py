import threading
from numpy import *
import copy

n = 5
m = 5

global new_grid
global next_gen
global cero_grid



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


                    if i + 1 < m :
                        if j-1 >= 0 and new_grid[j-1][i+1] == True:
                            poblacion = poblacion + 1

                        if new_grid[j][i+1] == True :
                            poblacion = poblacion + 1

                        if j+1 < n and new_grid[j+1][i+1] == True :
                            poblacion = poblacion + 1
                else :
                    indexIzquierda = m - 1 if i - 1 < 0 else i - 1
                    indexDerecha = 0 if i + 1 >= m else i + 1

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

    return 

def imprimir(temporal):

    for i in range(0, n):
        for j in range (0, n):
            if (temporal[i][j] == True) :
                print("*", end=' ')
            else :
                print("-", end=' ')
        print()

    print()
    print()
    print()
    print()
    


if __name__ =="__main__":

    cero_grid = [[False for j in range(n)] for i in range(n)]

    new_grid = copy.deepcopy(cero_grid)
    next_gen = copy.deepcopy(cero_grid)

    new_grid[0][1] = True
    new_grid[1][2] = True
    new_grid[2][0] = True
    new_grid[2][1] = True
    new_grid[2][2] = True

    for i in range(0, 6) :
        imprimir(new_grid)
        calculate_next_gen(0, n - 1, 0, n - 1, 0 , True)

        new_grid = copy.deepcopy(next_gen)
        next_gen = copy.deepcopy(cero_grid)

