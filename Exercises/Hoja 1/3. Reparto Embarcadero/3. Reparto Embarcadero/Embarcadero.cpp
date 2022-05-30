//Marcos Herrero

#include <iostream>
#include <vector>
#include <algorithm>

/*
El orden de llegada de los barcos y , por tanto, de embarque, es de 0 a N-1

Definicion recursiva: (0<= i <= N, 0<= j <= L)
maxBarcos(i,j) es el maximo numero de barcos desde el i al N-1 que pueden atracar cuando hay j espacio libre en la 
fila de la izquierda. En la fila de la derecha hay:
  - 0 espacio libre si (sum:0<= k <= i-1:e[k])  >= 2*L - j
  - 2*L-j - (sum:0<= k <= i-1:e[k]) espacio libre en caso contrario
  0<= i <= N, 0<=j <= L

Casos base:
maxBarcos(N, j) = 0 para todo 0<= j <= L
maxBarcos(i, j) = 0  si j < e[i] y (2*L- j - (sum:0<= k <= i-1:e[k]) < e[i])


Casos recursivos:
maxBarcos(i,j) = 1 + max( maxBarcos(i+1, j-e[i]), maxBarcos(i+1, j)) si j >=e[i] y (2*L- j - (sum:0<= k <= i-1:e[k]) >= e[i])
maxBarcos(i, j) = 1 + maxBarcos(i+1, j-e[i]) si j >= e[i] y (2*L- j - (sum:0<= k <= i-1:e[k]) < e[i])
maxBarcos(i,j) = 1 + maxBarcos(i+1, j) si j < e[i] y (2*L- j - (sum:0<= k <= i-1:e[k]) >= e[i])


*/


void resolver(std::vector<int> const & e, int L) {
	int N = e.size();
	std::vector<int> maxBarcos(L + 1, 0);

	int suma = 0;
	for (int k = 0; k < N; ++k)suma += e[k];

	for (int i = N - 1; i >= 0; --i) {
		suma -= e[i];

		for (int j = L; j >= 0; --j) {
			if (j >= e[i] && 2 * L - j - suma >= e[i])maxBarcos[j] = 1 + std::max(maxBarcos[j - e[i]], maxBarcos[j]);
			else if (j >= e[i] && 2 * L - j - suma < e[i]) maxBarcos[j] = 1 + maxBarcos[j-e[i]];
			else if (j < e[i] && 2 * L - j - suma >= e[i]) maxBarcos[j] = 1 + maxBarcos[j];
			else maxBarcos[j] = 0;
		}
	}

	std::cout << maxBarcos[L] << '\n';
}

//Coste O(N*L) en tiempo y O(L) en espacio

bool resuelveCaso() {
	int N, L;
	std::cin >> N >> L;
	if (N == 0)return false;
	
	std::vector<int> e(N);
	for (int i = 0; i < N; ++i) std::cin >> e[i];

	resolver(e, L);
}

int main() {
	while (resuelveCaso());
}