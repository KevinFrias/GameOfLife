#include <iostream>
#include <thread>
#include <mutex>
#include <random>
#include <chrono>

using namespace std;

const int n = 200; // Grid size.
bool matrix[n][n]; // Global grid.
std::mutex mtx; // Mutex for locking access to the grid.
bool bandera_nulo = true;

// Function that changes a random value in the grid.
void changeGrid(int x1, int x2, int y1, int y2, int iteracion){
  if (iteracion == 0){

		for (int i = x1; i <= x2; i++){
			for (int j = y1; j <= y2; j++) {
                int poblacion = 0;
                mtx.lock();

                if (bandera_nulo){
					// Lado izquierdo
					if (i - 1 >= 0){
						if (j - 1 >= 0) poblacion += (matrix[j - 1][i - 1]);
						poblacion += (matrix[j][i - 1]);
						if (j + 1 < n) poblacion += matrix[j + 1][i - 1];
					}
					// Lado central
					if (j - 1 >= 0) poblacion += matrix[j - 1][i];
					if (j + 1 < n) poblacion += matrix[j + 1][i];

					// Lado Derecho
					if (i + 1 < n){
						if (j - 1 >= 0) poblacion += matrix[j - 1][i + 1];
						poblacion += matrix[j][i + 1];
						if (j + 1 < n) poblacion += matrix[j + 1][i + 1];
					}
                }
				else{
                    int indexIzquierda = (i - 1 < 0 ? n - 1 : i - 1);
                    int indexDerecha = (i + 1 >= n ? 0 : i + 1);
                    
					int indexArriba = (j - 1 < 0 ? n - 1 : j - 1);
                    int indexAbajo = (j + 1 >= n ? 0 : j + 1);

					// Lado inzquierdo
					poblacion += matrix[indexArriba][indexIzquierda];
					poblacion += matrix[j][indexIzquierda];
					poblacion += matrix[indexAbajo][indexIzquierda];
					
					
					// Lado central
					poblacion += matrix[indexArriba][i];
					poblacion += matrix[indexAbajo][i];

					// Lado Derechp=o
					poblacion += matrix[indexArriba][indexDerecha];
					poblacion += matrix[j][indexDerecha];
					poblacion += matrix[indexAbajo][indexDerecha];

				}

            	if (matrix[j][i]){
					if (!(poblacion >= 4 || poblacion <= 1)) cout << j << ", " << i << "  |  *" << endl; 
                }

				else{
					if (poblacion == 3) cout << j << ", " << i << "  |  *" << endl; 
                }

                mtx.unlock();

			}
		}

		return;
	}

    int mitad_x = x2 / 2;
	int mitad_y = y2 / 2;

	thread cuadrantes[4];
	
	cuadrantes[0] = thread (changeGrid, x1, mitad_x, y1, mitad_y, iteracion - 1);
	cuadrantes[1] = thread (changeGrid, mitad_x  + 1, x2, y1, mitad_y, iteracion - 1);
	cuadrantes[2] = thread (changeGrid, x1, mitad_x, mitad_y  + 1, y2, iteracion - 1);
	cuadrantes[3] = thread (changeGrid, mitad_x + 1, x2, mitad_y + 1, y2, iteracion - 1);

	for (int i = 0; i < 4; i++) cuadrantes[i].join();
    
    return;
}

int main(){

    matrix[0][1] = true;
    matrix[1][2] = true;
    matrix[2][0] = true;
    matrix[2][1] = true;
    matrix[2][2] = true;

    int iteracion_total = 4;

    thread t(changeGrid, 0, n - 1, 0, n - 1, iteracion_total);
    t.join();


    return 0;
}
