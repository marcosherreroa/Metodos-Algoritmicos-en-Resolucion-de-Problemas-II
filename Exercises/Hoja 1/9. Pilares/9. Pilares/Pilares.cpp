//Marcos Herrero

#include <iostream>
#include <vector>

/*
Los ladrillos estan numerados de 0 a N-1

Definicion recursiva:

maxAltura(i) es un vector que recoge las alturas de los pilares que se consiguen con los ladrillos desde i hasta N-1 con la restriccion de que
la diferencia entre la altura maxima y la minima sea

*/

void resolver(std::vector<int> const & alturas) {
	int N = alturas.size();

	
}

bool resuelveCaso() {
	int N;
	std::cin >> N;
	if (N == 0)return false;

	std::vector<int> alturas(N);
	for (int i = 0; i < N; ++i)std::cin >> alturas[i];

	return true;
}

int main() {
	while (resuelveCaso());
}