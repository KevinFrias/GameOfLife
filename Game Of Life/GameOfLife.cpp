#include <bits/stdc++.h>
using namespace std;

const int n = 5, m = 5;
vector < vector <char>> matrix(n, vector(m, 'a'));

void next_gen(int x1, int x2, int y1, int y2, int iteracion){
	
	if (iteracion == 4){
		int temporal = 0;
		for (int i = x1; i <= x2; i++){
			for (int j = y1; j <= y2; j++) {

				// Primero obtenemos el numero de vecinos
				// 8 bits
				temporal = matrix[j][i] - 'a';
				int valor = (temporal & (1 << 2)) + ((temporal & (1 << 3)) << 1) +  ((temporal & (1 << 4)) << 2) + ((temporal & (1 << 5)) << 3);
			}
		}
		return ;
	}
	
	int mitad_x = x2 / 2;
	int mitad_y = y2 / 2;

	thread cuadrantes_1 (next_gen, x1, mitad_x, y1, mitad_y, iteracion + 1);
	thread cuadrantes_2 (next_gen, mitad_x  + 1, x2, y1, mitad_y, iteracion + 1);
	thread cuadrantes_3 (next_gen, x1, mitad_x, mitad_y  + 1, y2, iteracion + 1);
	thread cuadrantes_4 (next_gen, mitad_x + 1, x2, mitad_y + 1, y2, iteracion + 1);

	cuadrantes_1.join();
	cuadrantes_2.join();
	cuadrantes_3.join();
	cuadrantes_4.join();


	return;
}

void imprimir_bits(int a){

	for (int i = 0; i < 8; i++){
		if (a&1) cout << 1 << " ";
		else cout << 0 << " ";
		a = a >> 1;
	}
	cout << endl;
}

int main(){

//	next_gen(0, n-1, 0, m-1, 0);    

	matrix[0][0] = 'o';

	int temporal = matrix[0][0];
	cout << temporal << endl;
	cout << char(temporal) << endl;

	int valor = (bool)(temporal & (1 << 1)) + ((bool)(temporal & (1 << 2)) << 1) +  ((bool)(temporal & (1 << 3)) << 2) + ((bool)(temporal & (1 << 4)) << 3);
	cout << valor << endl;

	/*
	for (int i = 0; i < 8; i++){
		if (a&1) cout << 1 << " ";
		else cout << 0 << " ";
		a = a >> 1;
	}

	cout << endl << endl;

	for (int i = 0; i < 5; i++){
		for (int j = 0; j < 5; j++) {
			cout << matrix[i][j] << " ";


			if (!((matrix[i][j]-'a') & 1)) cout << 0 << " ";
			else cout << 1 << " ";


		}
		cout << endl;
	}
	
	*/
	
	return 0;
}
