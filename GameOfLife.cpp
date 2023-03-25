#include <bits/stdc++.h>
using namespace std;

const int n = 5, m = 5;
vector < vector <char>> matrix(n, vector(m, 'a'));
vector < vector <char>> matrix_next_gen(n, vector(m, 'a'));

// Matrix[n][m] -> Matrix[y][x] -> Matrix[j][i]


void next_gen(int x1, int x2, int y1, int y2, int iteracion, bool nulo){
	
	if (iteracion == 0){
		for (int i = x1; i <= x2; i++){
			for (int j = y1; j <= y2; j++) {
                int poblacion = 0;

                if (nulo){
					// Lado izquierdo
					if (i - 1 >= 0){
						if (j - 1 >= 0) poblacion += ((matrix[j - 1][i - 1] - 'a')&1);
						poblacion += ((matrix[j][i - 1] - 'a')&1);
						if (j + 1 < n) poblacion += ((matrix[j + 1][i - 1] - 'a')&1);
					}
					// Lado central
					if (j - 1 >= 0) poblacion += ((matrix[j - 1][i] - 'a')&1);
					if (j + 1 < n) poblacion += ((matrix[j + 1][i] - 'a')&1);

					// Lado Derecho
					if (i + 1 < m){
						if (j - 1 >= 0) poblacion += ((matrix[j - 1][i + 1] - 'a')&1);
						poblacion += ((matrix[j][i + 1] - 'a')&1);
						if (j + 1 < n) poblacion += ((matrix[j + 1][i + 1] - 'a')&1);
					}
                }
				else{
					// Arriba
                    int indexArriba = (j - 1 < 0 ? n - 1 : j - 1);
                    int indexAbajo = (j + 1 >= n ? 0 : j + 1);

                    int indexIzquierda = (i - 1 < 0 ? m - 1 : i - 1);
                    int indexDerecha = (i + 1 >= m ? 0 : i + 1);

					// Lado inzquierdo
					poblacion += ((matrix[indexArriba][indexIzquierda] - 'a')&1);
					poblacion += ((matrix[j][indexIzquierda] - 'a')&1);
					poblacion += ((matrix[indexAbajo][indexIzquierda] - 'a')&1);
					
					
					// Lado central
					poblacion += ((matrix[indexArriba][i] - 'a')&1);
					poblacion += ((matrix[indexAbajo][i] - 'a')&1);

					// Lado Derechp=o
					poblacion += ((matrix[indexArriba][indexDerecha] - 'a')&1);
					poblacion += ((matrix[j][indexDerecha] - 'a')&1);
					poblacion += ((matrix[indexAbajo][indexDerecha] - 'a')&1);

				}

				//cout << poblacion << endl;

				if ((matrix[j][i]-'a') & 1)
					if (poblacion >= 4 || poblacion <= 1)  matrix_next_gen[j][i] = 'a';
					else matrix_next_gen[j][i] = 'b';
				else 
					if (poblacion == 3) matrix_next_gen[j][i] = 'b';
			}
		}
		return;
	}
	
	int mitad_x = x2 / 2;
	int mitad_y = y2 / 2;

	thread cuadrantes_1 (next_gen, x1, mitad_x, y1, mitad_y, iteracion + 1, true);
	thread cuadrantes_2 (next_gen, mitad_x  + 1, x2, y1, mitad_y, iteracion + 1, true);
	thread cuadrantes_3 (next_gen, x1, mitad_x, mitad_y  + 1, y2, iteracion + 1, true);
	thread cuadrantes_4 (next_gen, mitad_x + 1, x2, mitad_y + 1, y2, iteracion + 1, true);

	cuadrantes_1.join();
	cuadrantes_2.join();
	cuadrantes_3.join();
	cuadrantes_4.join();


	return;
}


void imprimir(){
	cout << endl;
	for (int i = 0; i < n; i++){
		for (int j = 0; j < m; j++) cout << (matrix[i][j]-'a') << " ";
		cout << endl;
	}
	cout << endl << endl;
}


int main(){

	// Glider
	matrix[0][1] = 'b';
	matrix[1][2] = 'b';
	matrix[2][0] = 'b';
	matrix[2][1] = 'b';
	matrix[2][2] = 'b';


	imprimir();
	next_gen(0, n-1, 0, m-1, 0, true);    
	matrix = matrix_next_gen;
	imprimir();


	return 0;
}
