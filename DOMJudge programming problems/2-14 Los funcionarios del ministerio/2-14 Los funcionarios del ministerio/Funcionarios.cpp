//Marcos Herrero

#include <iostream>
#include <vector>
#include <queue>

/*
Restricciones explicitas:
Las soluciones son tuplas de la forma (s[0],...,s[N]) donde s[i] pertenece a {0,...,N-1}
s[i] = k representa que el funcionario i realiza la tarea k

Restricciones implicitas:
A cada funcionario se le ha de asignar exactamente una tarea

Objetivo:
Minimizar la suma de los tiempos que tardan los funcionarios en hacer las tareas

Cota optimista:
Se estima lo que tarda cada funcionario aun no asignado tardara como lo que tarda para la tarea que menos tarda
optimista = acum + sum: i<= k <= N: (min: 0<= j <= N: a[i][j]))
Se lleva precalculada la suma de minimos
Coste en cada llamada: O(1), Coste de precómputo: O(N^2), Coste en espacio adicional: O(N)

Cota pesimista:
Se completa la solucion parcial asignando los funcionarios que quedan en orden
Coste en cada llamada: O(N). No hay precómputo ni espacio adicional

*/

struct Nodo {
	int i;
	int acum;
	int optimista;
	std::vector<bool> asignadas;


	bool operator<(Nodo const & that)const {
		return optimista > that.optimista;
	}
};

int cotaOptimista(int i, int acum, std::vector<int> const & sumaRapido){
	return acum + sumaRapido[i];
}

int cotaPesimista(int i, int acum, std::vector<bool> const & asignadas,std::vector<std::vector<int>> const & tarda) {
	int n = tarda.size();

	int j = 0;
	while (i < n) {
		while (asignadas[j])++j;
		acum += tarda[i][j];
		++i;
		++j;
	}

	return acum;
}

void resolver(std::vector<std::vector<int>> const & tarda) {
	int N = tarda.size();

	std::vector<int> sumaRapido(N);
	int suma = 0, min;
	for (int i = N-1; i >= 0; --i) {
		min = tarda[i][0];
		for (int j = 0; j < N; ++j) {
			if (tarda[i][j] < min)min = tarda[i][j];
		}

		sumaRapido[i] = suma += min;
		suma = sumaRapido[i];
	}

	std::priority_queue<Nodo> pq;
	int minSuma = cotaPesimista(0, 0, std::vector<bool>(N, false), tarda);
	int pesimista;

	Nodo x = { 0,0, cotaOptimista(0,0,sumaRapido), std::vector<bool>(N,false) };
	pq.push(x);

	while (!pq.empty() && pq.top().optimista < minSuma) {
		x = pq.top();
		pq.pop();

		++x.i;
		for (int j = 0; j < N; ++j) {
			if (!x.asignadas[j]) {
				x.acum += tarda[x.i-1][j];
				x.asignadas[j] = true;

				if (x.i == N) {
					if (x.acum < minSuma)minSuma = x.acum;
				}
				else {
					x.optimista = cotaOptimista(x.i, x.acum, sumaRapido);

					if (x.optimista < minSuma) {
						pq.push(x);
						pesimista = cotaPesimista(x.i, x.acum, x.asignadas, tarda);
						if (pesimista < minSuma) minSuma = pesimista;
					}
				}

				x.acum -= tarda[x.i - 1][j];
				x.asignadas[j] = false;
			}
		}
	}

	std::cout << minSuma << '\n';
}

bool resuelveCaso() {
	int N;
	std::cin >> N;

	if (N == 0)return false;

	std::vector<std::vector<int>> tarda(N, std::vector<int>(N));
	for (int i = 0; i < N; ++i) {
		for (int j = 0; j < N; ++j) {
			std::cin >> tarda[i][j];
		}
	}

	resolver(tarda);

	return true;
}

int main() {
	while (resuelveCaso());
}