//Marcos Herrero
#include <iostream>
#include <vector>
#include <stack>

/*
Definicion recursivo:

minCoste(i,j) es el minimo coste de hacer el viaje de origen i y longitud j
0<= i <= N-1, 0<= j <= N-i-1

Casos base:
minCoste(i, 0) = 0 para todo 0<= i <= N-1

Casos recursivos:
minCoste(i,j) = min: 1<= k<= j: costes[i][k]+ minCoste(i+k, j-k)

*/

void resolver(std::vector<std::vector<int>> const & costes, int ini, int fin) {
	int N = costes.size() + 1;

	std::vector<std::vector<int>> minCoste(N, std::vector<int>(N));

	for (int i = 0; i <= N - 1; ++i) minCoste[i][0];

	for (int j = 1; j <= N - 1; ++j) {
		for (int i = 0; i <= N - j - 1; ++i) {
			minCoste[i][j] = costes[i][1] + minCoste[i + 1][j - 1];
			for (int k = 2; k <= j; ++k) {
				if (costes[i][k] + minCoste[i + k][j - k] < minCoste[i][j])minCoste[i][j] = costes[i][k] + minCoste[i + k][j - k];
			}
		}
	}

	std::vector<int> sol;
	int i = ini, j = fin - ini;
	while (j > 0) {
		int min = costes[i][1] + minCoste[i + 1][j - 1]; int len = 1;
		for (int k = 2; k <= j; ++k) {
			if (costes[i][k] + minCoste[i + k][j - k] < min) {
				min = costes[i][k] + minCoste[i + k][j - k];
				len = k;
			}
		}
		sol.push_back(i+len);
		i += len;
		j -= len;
	}

	for (int i = 0; i < sol.size(); ++i)std::cout << sol[i] << ' ';
	std::cout << '\n';
}

//Coste O(N^3) en tiempo y O(N^2) en espacio adicional

bool resuelveCaso() {
	int N, ini, fin;
	std::cin >> N>>ini>> fin;
	if (N == 0)return false;

	std::vector<std::vector<int>> costes(N - 1, std::vector<int>(N, 0));
	for (int i = 0; i < N - 1; ++i) {
		costes[i][0] = 0;
		for (int j = 1; j < N - i; ++j) { // j es la longitud del viaje
			std::cin >> costes[i][j];
		}
	}

	resolver(costes, ini, fin);

	return true;
}

int main() {
	while (resuelveCaso());
}