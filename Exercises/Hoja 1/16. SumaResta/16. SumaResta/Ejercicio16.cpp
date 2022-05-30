//Marcos Herrero

#include <iostream>
#include <vector>

/*
M = sum: 0<=k<=n-1: x[k], m = x[0] - sum:1<= k <= n-1: x[k]
Definicion recursiva:
posible(i, j) es true si es posible obtener el valor m+j poniendo signos a los terminos x[1],...x[i] y false en caso contrario
0<= i <= n-1, 0<= j <=  M-m 


Casos base:
posible(0, j) = true si x[0] = m+j y false en caso contrario
posible(i,j) = false si j-x[i] < 0 y j+x[i] > M-m

Casos recursivos:
posible(i,j) = posible(i-1,j-x[i])|| posible(i-1,j+x[i]) si j - x[i] >= 0 y j+x[i] <= M-m
posible(i,j) = posible(i-1, j-x[i])  si j-x[i] >= 0 y j +x[i] + > M-m
posible(i,j) = posible(i-1, j+x[i]) si j-x[i] < 0 y j + x[i] <= M-m

Creo que hay otra forma de hace

*/

void resolver(std::vector<int> const & x) {
	int n = x.size();

	int M = 0;
	for (int k = 0; k <= n - 1; ++k)M += x[k];
	int m = x[0];
	for (int k = 1; k <= n - 1; ++k)m -= x[k];

	if (m > 0 || M < 0)std::cout << "IMPOSIBLE\n";
	else {

		std::vector<std::vector<bool>> posible(n, std::vector<bool>(M - m + 1)); //parece que no se puede optimizar en espacio

		for (int j = 0; j <= M - m; ++j) {
			if (x[0] == m + j)posible[0][j] = true;
			else posible[0][j] = false;
		}

		for (int i = 1; i <= n - 1; ++i) {
			for (int j = 0; j <= M - m; ++j) {
				if (j - x[i] >= 0 && j + x[i] <= M - m) posible[i][j] = posible[i-1][j - x[i]] || posible[i-1][j + x[i]];
				else if (j - x[i] >= 0)posible[i][j] = posible[i-1][j - x[i]];
				else if (j + x[i] <= M - m) posible[i][j] = posible[i-1][j + x[i]];
				else posible[i][j] = false;
			}
		}


		if (posible[n-1][-m])std::cout << "POSIBLE\n";
		else std::cout << "IMPOSIBLE\n";
	}
}

//Coste O(n*(M-m)) en tiempo y  O(n*(M-m)) en espacio

bool resuelveCaso() {
	int n;
	std::cin >> n;
	if (n == 0)return false;

	std::vector<int> x(n);
	for (int i = 0; i < n; ++i)std::cin >> x[i];
	resolver(x);

	return true;
}

int main() {
	while (resuelveCaso());
}