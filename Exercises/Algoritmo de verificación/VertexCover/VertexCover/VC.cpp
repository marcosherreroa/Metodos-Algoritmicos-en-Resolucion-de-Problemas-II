//Marcos Herrero

#include "Grafo.h"
#include <set>

using namespace std;

bool verificaVC(Grafo const & G, int k, set<int> C) {
	if (C.size() > k)return false;

	bool OK = true;
	for (auto it = C.begin(); OK && it != C.end(); ++it) {
		if (*it < 0 || *it >= G.V())OK = false;
	}

	if (OK) {
		for (int v = 0; OK && v < G.V(); ++v) {
			if (C.count(v) == 0) {
				for (int i = 0; OK && i< G.ady(v).size(); ++i){
					if (C.count(G.ady(v)[i]) == 0)OK = false;
				}
			}
		}
	}

	return OK;
}

