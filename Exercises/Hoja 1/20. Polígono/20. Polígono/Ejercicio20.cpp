//Marcos Herrero

#include <iostream>
#include <vector>
#include <queue>

/*
Los vertices estan numerados de 0 a n-1

Definicion recursiva:
lenMinima(i,j) es la suma de longitudes minima al triangular el poligono de vertices i,i+1,...,i+j+1
0<= i <= n-1, 0<= j <= n-i-2 (j es el numero de lados/vertices del poligono menos 2. Lo hacemos asi para ahorrar espacio, ya que no puede haber poligonos de menos de tres lados,
                             pero conviene tener un caso base de poligonos de dos vertices para simplificar los casos recursivos)

Valor buscado: lenMinima(0,n-2)

Casos base:
lenMinima(i,0) = 0 para todo 0<= i <= n-1
lenMinima(i,1) = 0 para todo 0<= i <= n-1

Casos recursivos:
lenMinima(i,j) = min: 0 <= k <= j-1: d[i][i+k+1]+ d[i+k+1][j]+ lenMinima(i,k)+lenMinima(i+k+1,j-k-1))

*/

void resolver(std::vector<std::vector<int>> const & d) {
	int n = d.size();

	std::vector<std::vector<int>> lenMinima(n, std::vector<int>(n+1));
	std::vector<std::vector<int>> kMin(n, std::vector<int>(n + 1)); // para abaratar la reconstruccion

	for (int i = 0; i < n; ++i)lenMinima[i][0] = 0;
	for (int i = 0; i < n; ++i)lenMinima[i][1] = 0;


	for (int j = 2; j <= n-2; ++j) {
		for (int i = 0; i <= n - j-2; ++i) {
			lenMinima[i][j] = d[i + 1][j] + lenMinima[i + 1][j - 1];
			kMin[i][j] = 0;
			for (int k = 1; k <= j - 1; ++k) {
				if (d[i][i + k + 1] + d[i + k + 1][j] + lenMinima[i][k] + lenMinima[i + k + 1][j - k - 1] < lenMinima[i][j]) {
					lenMinima[i][j] = d[i][i + k + 1] + d[i + k + 1][j] + lenMinima[i][k] + lenMinima[i + k + 1][j - k - 1];
					kMin[i][j] = k;
				}
			}
		}
	}

	//Coste de rellenar las tablas: O(n^3) en tiempo y O(1) en espacio adicional

	std::vector<std::pair<int,int>> sol;
	std::queue<std::pair<int, int>> pendientes;
	pendientes.push({ 0,n - 2 });

	while (!pendientes.empty()) {
		int i = pendientes.front().first;
		int j = pendientes.front().second;
		pendientes.pop();

		if (j > 1) {
			if (kMin[i][j] == 0) {
				sol.push_back({ i + 1, i + j + 1 });
				pendientes.push({ i + 1,j - 1 });
			}
			else if (kMin[i][j] == j - 1) {
				sol.push_back({ i, i + j });
				pendientes.push({ i, j - 1 });
			}
			else {
				sol.push_back({ i, i + kMin[i][j] + 1 });
				sol.push_back({ i + kMin[i][j] + 1, j });
				pendientes.push({ i, kMin[i][j] });
				pendientes.push({ i + kMin[i][j] + 1, j - kMin[i][j] - 1 });
			}

		}
	}

	//Coste de reconstruir: O(n) en tiempo y O(1) en espacio adicional ( cada lado del poligono se procesa una vez a lo sumo)

	std::cout << lenMinima[0][n - 2] << '\n';
	for (int i = 0; i < sol.size(); ++i) {
		std::cout << sol[i].first << ' ' << sol[i].second << '\n';
	}

	std::cout << '\n';
}
//Coste total: O(n^2) en tiempo

bool resuelveCaso() {
	int n;
	std::cin >> n;
	if (!std::cin)return false;

	std::vector<std::vector<int>> d(n, std::vector<int>(n));
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) {
			std::cin >> d[i][j];
		}
	}

	resolver(d);

	return true;
}

int main() {
	while (resuelveCaso());
}