//Marcos Herrero

#include <iostream>
#include <vector>
#include <algorithm>
#include <stack>

/*
Definicion recursiva:

minCardinal(i,j) es el cardinal minimo de un subconjunto de elementos de {V[0],...,V[i-1]} cuya suma es j
                 , donde 0<= i <= N y 0 <= j <= C
Toma el valor +infinito si no existe tal subconjunto

Casos base:
  minCardinal(i,0)= 0 para todo 0<= i <= N
  minCardinal(0,j)= +infinito para todo 1 <= j <= N

Casos recursivos:
 minCardinal(i,j) = min( 1 + minCardinal(i-1, j- V[i-1]), minCardinal(i-1, j)) si  i >= 1 y j >= V[i-1]
 minCardinal(i,j) = minCardinal(i-1,j)                                         si  i >= 1 y  1 <=  j < V[i-1]

*/

void resolver(std::vector<int> const & V, int C) {
	int N = V.size();
	int infinity = N + 1;

	std::vector<std::vector<int>> tabla(N + 1, std::vector<int>(C + 1));
	for (int i = 0; i <= N; ++i)tabla[i][0] = 0;
	for (int j = 1; j <= C; ++j) tabla[0][j] = infinity;

	tabla[0][0] = 0;
	for (int i = 1; i <= N; ++i) {
		for (int j = 1; j <= C; ++j) {
			if (j >= V[i - 1])tabla[i][j] = std::min(1 + tabla[i - 1][j - V[i - 1]], tabla[i - 1][j]);
			else tabla[i][j] = tabla[i - 1][j];
		}
	}

	int cardMin = tabla[N][C];
	if (cardMin >= infinity)std::cout << "No existe tal subconjunto\n";
	else {

		int i = N, j = C;
		std::stack<int> sol;
		while (i > 0) { //reconstruccion
			if (j - V[i - 1] >= 0 && tabla[i - 1][j - V[i - 1]] < tabla[i - 1][j]) {
				j -= V[i - 1];
				sol.push(V[i - 1]);
			}
			--i;
		}

		while (!sol.empty()) {
			std::cout << sol.top() << ' ';
			sol.pop();
		}

		std::cout << '\n';
	}
}
// Coste O(N*C) en tiempo y O(N*C) en espacio adicional

bool resuelveCasoDP() {
	int N, C;
	std::cin >> N >> C;
	if (N == 0)return false;

	std::vector<int> V(N);
	for (int i = 0; i < N; ++i)std::cin >> V[i];

	resolver(V, C);
	return true;
}

int main() {
	while (resuelveCasoDP());
}