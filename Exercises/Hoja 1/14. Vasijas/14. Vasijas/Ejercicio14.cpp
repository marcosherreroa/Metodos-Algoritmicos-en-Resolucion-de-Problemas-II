//Marcos Herrero

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

/*
Definicion recursiva
posible(i,j,k) es true si es posible alcanzar la cantidad C en k pasos partiendo de tener i litros en la primera
vasija y j litros en la segunda, y es false en caso contrario
0<= i <= C1 , 0<= j <= C2, 0<= k <= n

Casos base:
posible(i,j, 0) = true si i = C o j= C
posible (i,j, 0) = false si i != C y j != C

Casos recursivos:
posible(i,j,k) = posible(C1, j, k-1) || posible(i, C2, k-1) || posible(0, j, k-1) || posible(i, 0, k-1) || posible(0, j, k-1)
                 || posible(i-min(i,C2-j), j + min(i,C2-j), k-1) || posible(i+min(j, C1-i), j - min(j, C1-i), k-1)   si k > 0

*/

void resolver(int C, int C1, int C2, int n) {
	std::vector<std::vector<std::vector<bool>>> posible(C1 + 1, std::vector<std::vector<bool>>(C2 + 1, std::vector<bool>(n + 1)));

	for (int i = 0; i <= C1; ++i) {
		for (int j = 0; j <= C2; ++j) {
			if (i == C || j == C)posible[i][j][0] = true;
			else posible[i][j][0] = false;
		}
	}

	for (int k = 1; k <= n; ++k) {
		for (int i = 0; i <= C1; ++i) {
			for (int j = 0; j <= C2; ++j) {
				posible[i][j][k] = posible[C1][j][k - 1] || posible[i][C2][k - 1] || posible[0][j][k - 1] ||posible[i][0][k - 1] || 
					posible[i - std::min(i, C2 - j)][j + std::min(i, C2 - j)][k - 1] || posible[i + std::min(j, C1 - i)][j - std::min(j, C1 - i)][k - 1];
			}
		}
	}

	// se podria añadir una accion de "No hacer nada" pero esto es lo mismo que llenar una vasija cuando ya esta llena

	if (!posible[0][0][n])std::cout << "No es posible\n";
	else {
		int i = 0, j = 0, k = n;

		std::vector<std::string> acciones(n);
		while (k > 0) {
			if (posible[C1][j][k - 1]) {
				acciones[n-k] = "Llenar 1";
				i = C1;
			}
			else if (posible[i][C2][k - 1]) {
				acciones[n-k] = "Llenar 2";
				j = C2;
			}
			else if (posible[0][j][k - 1]) {
				acciones[n - k] = "Vaciar 1";
				i = 0;
			}
			else if (posible[i][0][k - 1]) {
				acciones[n - k] = "Vaciar 2";
				j = 0;
			}
			else if (posible[i - std::min(i, C2 - j)][j + std::min(i, C2 - j)][k - 1]) {
				acciones[n - k] = "Trasvasar de 1 a 2";
				int aux = std::min(i, C2 - j);
				i -= aux;
				j += aux;
			}
			else {
				acciones[n - k] = "Trasvasar de 2 a 1";
				int aux = std::min(j, C1 - i);
				i -= aux;
				j += aux;
			}

			--k;
		}

		for (int i = 0; i < n; ++i) std::cout << acciones[i] << '\n';
		
		std::cout << '\n';
	}
}

// Coste O(n*C1*C2) en tiempo y O(n*C1*C2) en espacio adicional

bool resuelveCaso() {
	int C, C1, C2, n;
	std::cin >> C >> C1 >> C2 >> n;
	if (C == 0)return false;

	resolver(C, C1, C2, n);
	return true;
}

int main() {
	while (resuelveCaso());
}