//Marcos Herrero

#include <iostream>
#include <vector>

/*
Renumero las huertas para que vayan de 0 a m-1

Definicion recursiva:
maxBen(i,j) es el beneficio máximo que se puede obtener de las huertas 0 a i-1 disponiendo de j dias de trabajo
0<= i <= m, 0<= j <= n

Casos base:
maxBen(0,j) = 0 para todo 0<= j <= n

Casos recursivos:
maxBen(i,j) = max: 0<= k <= j: b[i-1][k]+ maxBen(i-1,j-k)  

*/

void resolver(std::vector<std::vector<int>> const & b) {
	int m = b.size();
	int n = b[0].size() - 1;

	std::vector<std::vector<int>> maxBen(m + 1, std::vector<int>(n + 1));

	for (int j = 0; j <= n; ++j)maxBen[0][j] = 0;

	for (int i = 1; i <= m; ++i) {
		for (int j = 0; j <= n; ++j) {
			int max = b[i - 1][0] + maxBen[i - 1][j];
			for (int k = 1; k <= j; ++k) {
				if (b[i - 1][k] + maxBen[i - 1][j - k] > max)max = b[i - 1][k] + maxBen[i - 1][j - k];
			}

			maxBen[i][j] = max;
		}
	}

	std::vector<int> diasHuerta(m);
	int i = m, j = n;
	while (i > 0) {
		int max = b[i - 1][0] + maxBen[i - 1][j];
		int dias = 0;
		for (int k = 1; k <= j; ++k) {
			if (b[i - 1][k] + maxBen[i - 1][j - k] > max) {
				max = b[i - 1][k] + maxBen[i - 1][j - k];
				dias = k;
			}
		}

		diasHuerta[i - 1] = dias;
		j -= dias;
		--i;
	}

	for (int i = 0; i < m; ++i) std::cout << diasHuerta[i] << ' ';
	std::cout << '\n';
}

// Coste O(m*n^2) en tiempo y O(m*n) en espacio adicional

bool resuelveCaso() {
	int m, n;
	std::cin >> m >> n;
	if (n == 0 && m == 0)return false;

	std::vector<std::vector<int>> b(m, std::vector<int>(n+1));
	for (int i = 0; i < m; ++i) {
		for (int j = 0; j <= n; ++j) {
			std::cin >> b[i][j];
		}
	}

	resolver(b);

	return true;
}

int main() {
	while (resuelveCaso());
}