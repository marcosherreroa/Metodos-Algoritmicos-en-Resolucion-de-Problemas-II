//Marcos Herrero

#include <iostream>
#include <vector>

/*
Definicion recursiva:
 
 maxMonedas(i,j) es el maximo numero de monedas la 0 a la i-1 que se pueden usar para pagar la cantidad j
 0<= i <= n , 0<= j <= C

 Casos base:
 maxMonedas(i,0) = 0 para todo 0<= i <= n
 maxMonedas(0,j) = -infinity para todo 1<= j <= C

 Casos recursivos:
    maxMonedas(i,j) = max: 0<= k <= num[i] && k*valor[i] <= j: k + maxMonedas(i-1, j-k*valor[i])
       para 1<= i <= n; 1<= j <= C 

*/

void resolver(int C , std::vector<int> const & valor, std::vector<int> const & num) {
	int n = valor.size(); 
	int minusInf = -1000000;
	

	std::vector<std::vector<int>> maxMonedas(n + 1, std::vector<int>(C + 1));

	for (int i = 0; i <= n; ++i)maxMonedas[i][0]= 0;
	for (int j = 1; j <= C; ++j)maxMonedas[0][j]= minusInf;

	for (int i = 1; i <= n; ++i) {
		for (int j = 1; j <= C; ++j) {
			int max = maxMonedas[i - 1][j];
			for (int k = 1; k <= num[i-1] && k*valor[i-1] <= j; ++k) {
				if (k + maxMonedas[i - 1][j - k * valor[i-1]] > max) {
					max = k + maxMonedas[i - 1][j - k * valor[i-1]];
				}
			}

			maxMonedas[i][j] = max;
		}
	}

	if (maxMonedas[n][C] >= 0) {
		std::vector<int> monedasPagadas(n);

		int i = n, j = C;
		while (i > 0) {
			int max = maxMonedas[i - 1][j], monedas = 0;
			for (int k = 1; k <= num[i - 1] && k*valor[i - 1] <= j; ++k) {
				if (k + maxMonedas[i - 1][j - k * valor[i - 1]] > max) {
					max = k + maxMonedas[i - 1][j - k * valor[i - 1]];
					monedas = k;
				}
			}

			monedasPagadas[i - 1] = monedas;
			j -= monedas * valor[i - 1];
			--i;
		}

		for (int i = 0; i < n; ++i)std::cout << monedasPagadas[i] << ' ';
		std::cout << '\n';
	}

	else std::cout << "No se puede\n";
}

//Coste O(M*C) en tiempo y O(n*C) en espacio adicional, donde M es el numero total de mondeas de que disponemos

bool resuelveCaso() {
	int n, C;
	std::cin >> n >> C;
	if (n == 0 && C == 0)return false;

	std::vector<int> num(n), valor(n);
	for (int i = 0; i < n; ++i)std::cin >> valor[i]>> num[i];

	resolver(C, valor, num);

	return true;
}

int main() {
	while (resuelveCaso());
}