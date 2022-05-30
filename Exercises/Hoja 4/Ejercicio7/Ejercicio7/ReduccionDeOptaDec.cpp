//Marcos Herrero

#include <iostream>
#include <vector>
#include "Grafo.h"

using namespace std;

Grafo eliminarVertice()

bool PDC(Grafo const & G, int n) {
	return true;
}

vector<int> PC(Grafo const & G) {
	if (G.V() == 0) return vector<int>();

	int ini = 1, T = G.V(), m;
	while (ini < T) {
		m = (ini + T) / 2;
		if (PDC(G, m))ini = m;
		else T = m + 1;
	}

	Grafo G1 = G;
	vector<int> clique;
	vector<pair<int, int>> quitadas;
	for (int v = 0; v < G.V(); ++v) {
		for (int w : G1.ady(v)) {
			G1.eliminaArista(v, w);
			quitadas.push_back({ v,w });
		}

		if (!PDC(G, T)) {
			clique.push_back(v);
			for (int i = 0; i < quitadas.size(); ++i) {
				G1.ponArista(quitadas[i].first, quitadas[i].second);
			}
		}

		quitadas.clear();
	}

	return clique;
}