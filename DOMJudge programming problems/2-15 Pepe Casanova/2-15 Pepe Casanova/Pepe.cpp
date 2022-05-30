//Marcos Herrero

#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>


struct cancion {
	double dur;
	double punt;

	bool operator<(cancion const & that)const {
		return punt / dur > that.punt / that.dur;
	}
};

/*
Restricciones explicitas:
Las soluciones son tuplas (s[0],...,s[n-1]) donde s[i] pertenece a {0,1,2}
s[i] = 0 representa que la cancion no se almacena en ninguna cara, s[i] = 1 que se almacena en la primera y s[i] = 2 que se almacena en la seginda

Restricciones implicitas:
La suma de los espacios de las canciones almacenadas en la primera cara y la suma de los espacios de las canciones almacenadas en la segunda cara
no deben superar espacioCara

Objetivo:
Maximizar la suma de las puntuaciones asociada a las canciones guardadas

Cota optimista:
Se toman las fracciones como fraccionables , y se rellenan ambas caras del disco de forma voraz. Para ello, conviene tener las canciones ordenadas
por puntuacion/espacio, de mayor a menor

Cota pesimista:
Se estima por 0 laspuntuaciones de las canciones aun no guardadas
Coste en cada llamada: O(1). No hay precomputo ni espacio adicional


Nota: como solo se pide el valor de la funcion objetivo es mejor no llevar la solucion en el nodo, para ahorrar espacio
*/


struct Nodo {
	int i;
	double puntAcum;
	double espLibre1;
	double espLibre2;
	double optimista;

	bool operator<(Nodo const & that)const {
		return optimista < that.optimista;
	}
};

double cotaOptimista(int i, double puntAcum, double espLibre1, double espLibre2, std::vector<cancion> const & canciones) {
	int N = canciones.size();
	int espLibre = espLibre1 + espLibre2;

	while (i < N && canciones[i].dur <= espLibre) {
		puntAcum += canciones[i].punt;
		espLibre -= canciones[i].dur;
		++i;
	}
	if (i < N) puntAcum += espLibre / canciones[i].dur * canciones[i].punt;

	return puntAcum;
}

double cotaPesimista(int i, double puntAcum, double espLibre1, double espLibre2) {
	return puntAcum;
}

void resolver(double const & espacioCara, std::vector<cancion> const & canciones) {
	int N = canciones.size();

	std::priority_queue<Nodo> pq;
	double maxPunt = 0;
	double pesimista;

	Nodo x = { 0, 0,espacioCara, espacioCara, cotaOptimista(0,0,espacioCara,espacioCara,canciones) };
	pq.push(x);

	while (!pq.empty() && pq.top().optimista > maxPunt) {
		x = pq.top();
		pq.pop();

		++x.i;

		if (canciones[x.i - 1].dur <= x.espLibre1) {//primera cara
			x.puntAcum += canciones[x.i - 1].punt;
			x.espLibre1 -= canciones[x.i - 1].dur;

			if (x.i == N) {
				if (x.puntAcum > maxPunt)maxPunt = x.puntAcum;
			}
			else {
				//x.optimista = cotaOptimista(x.i, x.puntAcum, x.espLibre1,x.espLibre2, canciones);// igual rentaria una forma en la que no hubiera que recalcular

				if (x.optimista > maxPunt) {
					pq.push(x);
					//pesimista = cotaPesimista(x.i, x.puntAcum, x.espLibre1, x.espLibre2);
					//if (pesimista > maxPunt)maxPunt = pesimista;
				//}

			}

			x.puntAcum -= canciones[x.i - 1].punt;
			x.espLibre1 += canciones[x.i - 1].dur;
		}

		if (canciones[x.i - 1].dur <= x.espLibre2) {//segunda cara
			x.puntAcum += canciones[x.i - 1].punt;
			x.espLibre2 -= canciones[x.i - 1].dur;

			if (x.i == N) {
				if (x.puntAcum > maxPunt)maxPunt = x.puntAcum;
			}
			else {
				//x.optimista = cotaOptimista(x.i, x.puntAcum, x.espLibre1,x.espLibre2, canciones);

				if (x.optimista > maxPunt) {
					pq.push(x);
					//pesimista = cotaPesimista(x.i, x.puntAcum, x.espLibre1, x.espLibre2);
					//if (pesimista > maxPunt)maxPunt = pesimista;
				}

			}

			x.puntAcum -= canciones[x.i - 1].punt;
			x.espLibre2 += canciones[x.i - 1].dur;
		}

		if (x.i == N) {//Ninguna cara
		   if (x.puntAcum > maxPunt)maxPunt = x.puntAcum;
		}
		else {
			x.optimista = cotaOptimista(x.i, x.puntAcum, x.espLibre1, x.espLibre2, canciones);

			if (x.optimista > maxPunt) {
				pq.push(x);
				pesimista = cotaPesimista(x.i, x.puntAcum, x.espLibre1, x.espLibre2);
				if (pesimista > maxPunt)maxPunt = pesimista;
			}
		}
	}

	std::cout << maxPunt << '\n';
}

bool resuelveCaso() {
	int N; double espacioCara;
	std::cin >> N;
	if (N == 0)return false;
	std::cin >> espacioCara;

	std::vector<cancion> canciones(N);
	for (int i = 0; i < N; ++i)std::cin >> canciones[i].dur >> canciones[i].punt;
	std::sort(canciones.begin(), canciones.end());

	resolver(espacioCara,canciones);

	return true;
}

int main() {
	while (resuelveCaso());
}