//Marcos Herrero

#include <iostream>
#include <vector>
#include <algorithm>

/*
Def:
maxValor(i,j) es maximo valor que podemos acumular si estamos en la casilla (i,j)
0<= i <= N-1, 0<= j <=N-1

maxValor(0,j) = tablero[0][j] para todo 0<= j <= N-1;


maxValor(i,j) = max ( maxValor(i-1, j-1), maxValor(i-1, j), maxValor(i-1, j)) si 0<j < N-1
maxValor(i,j) = max ( maxValor(i-1,0), maxValor(i-1,1)) si j= 0
maxValor(i,j) = max( maxValor(i-1, N-2), maxValor(i-1,N-1)) si j = N-1

*/
void resolver(std::vector<std::vector<int>> const & tablero) {
	int N = tablero.size();

	std::vector<std::vector<int>> maxValor(N, std::vector<int>(N)); //creo que no se puede optimizar en espacio. Espacio adicional: O(N^2)
	for (int j = 0; j <= N - 1; ++j) maxValor[0][j] = tablero[0][j];

	for (int i = 1; i <= N - 1; ++i) {
		for (int j = 0; j <= N - 1; ++j) {
			if (j == 0) maxValor[i][j] = tablero[i][j]+ std::max(maxValor[i - 1][0], maxValor[i - 1][1]);
			else if (j == N - 1)maxValor[i][j] = tablero[i][j] + std::max(maxValor[i - 1][N - 2], maxValor[i - 1][N - 1]);
			else maxValor[i][j] = tablero[i][j] + std::max(maxValor[i - 1][j - 1], std::max(maxValor[i - 1][j], maxValor[i - 1][j + 1]));
		}
	}
	//Coste de rellenar la tabla: O(N^2) en tiempo y O(1) en espacio adicional

	int indmax = 0, max = maxValor[N-1][0];
	for (int j = 1; j < N; ++j) {
		if (maxValor[N - 1][j] > max) {
			max = maxValor[N - 1][j];
			indmax = j;
		}
	}
	//Coste de hallar la casilla de inicio: O(N) en tiempo y O(1) en esapcio adicional

	std::cout << max << ' ' << indmax+1 << '\n';
}
//Coste total: O(N^2) en tiempo y O(N^2) en espacio adicional

bool resuelveCaso() {
	int N;
	std::cin >> N;
	if (!std::cin)return false;

	std::vector<std::vector<int>> tablero(N, std::vector<int>(N));

	for (int i = 0; i < N; ++i) {
		for (int j = 0; j < N; ++j) {
			std::cin >> tablero[i][j];
		}
	}

	resolver(tablero);

	return true;
}

int main() {
	while (resuelveCaso());
}