//Marcos Herrero

#include <iostream>
#include <vector>

/*

Definicion recursiva:
numArboles(i) es el numero de arboles binarios de busqueda que se pueden formar con i claves
0<= i <= n

Casos Base:
numArboles(0) = 1 ( el arbol vacio)

Casos recursivos:
numArboles(i) = sum: 1<= k <= i: numArboles(k-1)*numArboles(i-k-1)


*/

void resolver(int n) {
	std::vector<int> numArboles(n + 1);
	numArboles[0] = 1;

	for (int i = 1; i <= n; ++i) {
		numArboles[i] = 0;
		for (int k = 0; k <= i-1; ++k) {
			numArboles[i] += numArboles[k] * numArboles[i - k - 1];
		}
	}

	std::cout << numArboles[n] << '\n';
}
//Coste O(n^2) en tiempo y O(n) en espacio adicional

bool resuelveCaso() {
	int n;
	std::cin >> n;
	if (n == 0)return false;

	resolver(n);

	return true;
}
int main() {
	while (resuelveCaso());
}