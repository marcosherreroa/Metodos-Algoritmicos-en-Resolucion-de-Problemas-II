//Marcos Herrero

#include <iostream>
#include <vector>

/*
Renumeramos las playas de 0 a m-1

Definicion recursiva:
maxKilos(i,j) es el maximo numero de kilos de basuras que se podrian recoger repartiendo los voluntarios de 0 a i-1 entre las playas de 0 a j-1
0<=i<=n, 0<=j<=m

Casos base:
maxKilos(i,0)= 0 para todo 0<= i <= n

Casos recursivos:
maxKilos(i,j)= max :0<= k <= i: B[k][j-1] + maxKilos(i-k,j-1)


*/


void resolver(std::vector<std::vector<int>> const & B) {
	int n = B.size() - 1, m = B[0].size();

	std::vector<std::vector<int>> maxKilos(n + 1, std::vector<int>(m + 1));

	for (int i = 0; i <= n; ++i)maxKilos[i][0] = 0;

	for (int j = 1; j <= m; ++j) {
		for (int i = 0; i <= n; ++i) {
			maxKilos[i][j] = B[0][j - 1]+ maxKilos[i][j-1];
			for (int k = 0; k <= i; ++k) {
				if (B[k][j - 1] + maxKilos[i-k][j - 1] > maxKilos[i][j]) maxKilos[i][j] = B[k][j - 1] + maxKilos[i-k][j - 1];
			}
		}
	}

	std::vector<int> sol(m); //voluntarios por playa
	int j = m, i = n;
	while (j > 0) {
		int kilos = B[0][j - 1] + maxKilos[i][j - 1], vols = 0;
		for (int k = 0; k <= i; ++k) {
			if (B[k][j - 1] + maxKilos[i-k][j - 1] > kilos) {
				kilos = B[k][j - 1] + maxKilos[i-k][j - 1];
				vols = k;
			}
		}

		sol[j - 1] = vols;
		i -= vols;
		j--;
	}

	std::cout << maxKilos[n][m] << '\n';
	
	for (int i = 0; i < m; ++i)std::cout << sol[i] << ' ';
	std::cout << '\n';
}

//Coste O(m*n^2) en tiempo y O(m*n) en espacio adicional

bool resuelveCaso() {
	int n, m;
	std::cin >> n >> m;
	if (n == 0)return false;

	std::vector<std::vector<int>> B(n+1, std::vector<int>(m));
	
	for (int i = 0; i <= n; ++i) {
		for (int j = 0; j < m; ++j) {
			std::cin >> B[i][j];
		}
	}

	resolver(B);

	return true;
}

int main() {
	while (resuelveCaso());
}