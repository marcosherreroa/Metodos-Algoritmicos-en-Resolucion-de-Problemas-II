//Marcos Herrero

#include <iostream>
#include <vector>
#include "Grafo.h"

using namespace std;

bool verificaPDC(Grafo const & G, int k, vector<int> const & C) {//Coste O(V^2)
	if (C.size() < k)return false;

	bool OK = true;

	for (int i = 0; OK && i < C.size(); ++i) {
		if (C[i] <  0 || C[i] >= G.V())OK = false;
		else {
			for (int j = 0; j < i; ++j) {
				if (!G.connected(i, j))OK = false;
			}
		}
	}

	return OK;
}

bool verificaPDC(Grafo const & G, int k, vector<bool> const & C) {//Coste O(V^2)
	if (C.size() != G.V()) return false;

	bool OK = true;
	for (int i = 0; OK && i < G.V(); ++i) {
		for (int j = 0; OK && j < i; ++j) {
			if (C[i] && C[j] && !G.connected(i, j)) OK = false;
		}
	}
	
	return OK;
}