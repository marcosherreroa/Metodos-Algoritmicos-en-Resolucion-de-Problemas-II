///Marcos Herrero

#include <iostream>
#include <vector>

/*
Definicion recursiva:

 minMonedas(i,j) es el minimo numero de monedas la i a la n-1 que se pueden usar para pagar la cantidad j
 0<= i <= n , 0<= j <= C

 Casos base:
 minMonedas(i,0) = 0 para todo 0<= i <= n
 minMonedas(n,j) = infinity para todo 1<= j <= C

 Casos recursivos:
	minMonedas(i,j) = max: 0<= k <= num[i] && k*valor[i] <= j: k + maxMonedas(i+1, j-k*valor[i])
	   para 1<= i <= n; 1<= j <= C

*/

void resolver(int C, std::vector<int> const & valor, std::vector<int> const & num) {
	int n = valor.size();
	int inf = 1000000000;

	std::vector<std::vector<int>> minMonedas(n + 1, std::vector<int>(C + 1));

	for (int i = 0; i <= n; ++i)minMonedas[i][0] = 0;
	for (int j = 1; j <= C; ++j)minMonedas[n][j] = inf;

	for (int i = n - 1; i >= 0; --i) {
		for (int j = 1; j <= C; ++j) {
			int min = minMonedas[i + 1][j];
			for (int k = 1; k <= num[i] && k*valor[i] <= j; ++k) {
				if (k + minMonedas[i + 1][j - k * valor[i]] < min) {
					min = k + minMonedas[i + 1][j - k * valor[i]];
				}
			}

			minMonedas[i][j] = min;
		}
	}

	if (minMonedas[0][C] < inf) std::cout << "SI " << minMonedas[0][C] << '\n';
		/*std::vector<int> monedasPagadas(n);

		int i = 0, j = C;

		while (i < n) {
			int min = minMonedas[i + 1][j], monedas = 0;
			for (int k = 1; k <= num[i] && k*valor[i] <= j; ++k) {
				if (k + minMonedas[i + 1][j - k * valor[i]] <= min) {
					min = k + minMonedas[i + 1][j - k * valor[i]];
					monedas = k;
				}
			}

			monedasPagadas[i] = monedas;
			j -= monedas * valor[i];
			++i;
		}

		std::cout << "SI";
		for (int i = 0; i < n; ++i)std::cout << ' ' << monedasPagadas[i];
		std::cout << '\n';
	}*/
	else std::cout << "NO\n";
}

//Coste O(M*C) en tiempo y O(n*C) en espacio adicional, donde M es el numero total de mondeas de que disponemos

bool resuelveCaso() {
	int n, C;
	std::cin >> n;
	if (!std::cin)return false;

	std::vector<int> num(n), valor(n);
	for (int i = 0; i < n; ++i)std::cin >> valor[i];
	for (int i = 0; i < n; ++i)std::cin >> num[i];

	std::cin >> C;

	resolver(C, valor, num);

	return true;
}

int main() {
	std::ios_base::sync_with_stdio(false);
	while (resuelveCaso());
}
