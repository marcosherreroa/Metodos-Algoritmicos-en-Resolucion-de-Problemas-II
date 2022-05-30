//Marcos Herrero

#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;

/*
maxValor(i,j) es el maximo valor que se puede guardar en la mochila con capacidad j considerando los objetos 0 a i-1 y  
0<= i <= n, 0<= j <= W

maxValor(0,j) = 0;
maxValor(i, 0) = 0;

Si i > 0:
* Si j >= peso[i-1]:
  maxValor(i,j) = max(valores[i-1]+  maxValor(i-1, j- pesos[i-1]), maxValor(i-1,j)
*/

int Mochila(vector<int> const & pesos, vector<int> const & valores, int W, vector<int> & sol) {
	int n = pesos.size();

	vector<vector<int>> maxValor(n, vector<int>(W));

	for (int j = 0; j <= W; ++j)maxValor[0][j] = 0;
	for (int i = 1; i <= n; ++i)maxValor[i][0] = 0;

	for (int i = 1; i <= n; ++i) {
		for (int j = 1; j < pesos[i-1]; ++j) {
			maxValor[i][j] = maxValor[i - 1][j];
		}

		for (int j = pesos[i - 1]; j <= W; ++j) {
			maxValor[i][j] = max(valores[i - 1] + maxValor[i - 1][j - pesos[i - 1]], maxValor[i - 1][j]);
		}
	}

	int i = n, j = W;
	while (i > 0 && j > 0) {
		if (j >= pesos[i - 1] && valores[i - 1] + maxValor[i - 1][j - pesos[i - 1]] >= maxValor[i - 1][j]) {
			sol.push_back(i);
			j -= pesos[i - 1];
		}

		--i;
	}

	return maxValor[n][W];
}