//Marcos Herrero
#include <iostream>
#include <vector>
#include <algorithm>

/*
Renumeramos los datos
Los vecinos estan numerados de 0 a n-1

Definicion recursiva

maxBen(i,j) es el máximo beneficio que se puede obtener al vender prendas a los vecinos 0,...,i-1 cuando se tienen j prendas en total
0<= i <= n, 0<=j <= P

Casos base:
maxBen(i,0) = 0 para 0<=i <= n
maxBen(0,j) = 0 para 0<= j <= n

Casos recursivos:
maxBen(i,j) = max: 0<= k <= min{h, j}: maxBen(i-1, j-k}

*/

void resolver(int P, std::vector<std::vector<int>> const & b) {
	int n = b.size();
	int h = b[0].size() - 1;

	std::vector<std::vector<int>> maxBen(n + 1, std::vector<int>(P + 1));

	for (int i = 0; i <= n; ++i) maxBen[i][0] = 0;
	for (int j = 1; j <= P; ++j)maxBen[0][j] = 0;

	for (int i = 1; i <= n; ++i) {
		for (int j = 1; j <= P; ++j) {
			int max = b[i-1][0] + maxBen[i-1][j]; int l = std::min( h, j );
			for (int k = 1; k <= l; ++k) {
				if (b[i-1][k]+ maxBen[i - 1][j - k] > max)max = b[i-1][k]+ maxBen[i - 1][j - k];
			}
			maxBen[i][j] = max;
		}
	}

	std::vector<int> prendasVecino(n,0);
	int i = n, j = P;
	while (i > 0) {
		int l = std::min(h, j);
		int prendas = 0, beneficio = b[i-1][0] + maxBen[i-1][j];
		for (int k = 0; k <= l; ++k) {
			if (b[i - 1][k] + maxBen[i - 1][j - k] > beneficio) {
				beneficio = b[i-1][k] + maxBen[i - 1][j - k];
				prendas = k;
			}
		}
		prendasVecino[i-1] = prendas;
		--i;
		j -= prendas;
	}

	for (int i = 0; i < n; ++i)std::cout << prendasVecino[i] << ' ';
	std::cout << '\n';
}

//Coste O(n*P*h) en tiempo y O(n*P) en espacio adicional

bool resuelveCaso() {
	int P, h, n;
	std::cin >>P>> h >> n;
	if (P == 0 && h == 0 && n == 0)return false;

	std::vector<std::vector<int>> b(n, std::vector<int>(h+1));

	for (int i = 0; i < n; ++i) {// b[i][j] es el beneficio obtenido al vender al vecino i, j prendas(aunque sean 0)
		for (int j = 0; j <= h; ++j) {
			std::cin >> b[i][j];
		}
	}

	resolver(P, b);

	return true;
}

int main() {
	while (resuelveCaso());
}