//Marcos Herrero
#include <iostream>
#include <vector>
#include <algorithm>

//Funcion recursiva: coste 2^n

int maxComer(std::vector<int> const & p, int quedan, int fin) {//solo hace falta fuardarse el fin, porque la longitud ya esta determinada, jaja
	if (quedan == 0)return 0;
	else {
		int s1 = maxComer
		if (p[ini + 1] > p[fin-1])s1 = maxComer(p,ini+2,fin); 
		else s1 = maxComer(p, ini + 1, fin - 1);

		int s2;
		if (p[ini] > p[fin - 2])s2 = maxComer(p, ini + 1, fin - 1);
		else s2 = maxComer(p, ini, fin - 2);

		return std::max(s1 + p[ini], s2 + p[fin - 1]);
	}
}

//Algoritmo iterativo:

std::vector<int> resolver(std::vector<int> const & p) {
	int n = p.size();

	
	for(int i = 0; i)
}


bool resuelveCaso() {
	int n;
	std::cin >> n;
	if (n == 0)return false;

	std::vector<int> p(n);
	for (int i = 0; i < n; ++i) std::cin >> p[i];

	return true;
}

int main() {
	while (resuelveCaso());
}