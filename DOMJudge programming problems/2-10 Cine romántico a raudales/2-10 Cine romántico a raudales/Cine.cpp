//Marcos Herrero

#include <iostream>
#include <vector>
#include <algorithm>

struct pelic {
	int empieza;
	int dur;

	bool operator<(pelic const & that)const {
		return empieza < that.empieza;
	}
};

struct indDur {
	int ind;
	int termina;

	bool operator<(indDur const & that)const  {
		return termina < that.termina;
	}
};

/*
Def:
maxTiempo(i) es el maximo tiempo que puede estar viendo peliculas considerando las peliculas de la i a la N-1 


maxTiempo(i) = max(pelic[i].dur+ maxTiempo(siguienteQuesepueda), maxTiempo(i+1))
*/

void resolver(std::vector<pelic> const & pelic) {
	int N = pelic.size();

	std::vector<int> siguiente(N);
	std::vector<indDur> pelicAux(N);
	for (int i = 0; i < N; ++i) pelicAux[i] = { i, pelic[i].empieza + pelic[i].dur };
	std::sort(pelicAux.begin(), pelicAux.end());
	//Coste de ordenar: O(NlogN) en tiempo y O(1) en espacio adicional

	int i= 1, j = 0;
	while (i < N) {
		while (i < N && pelicAux[j].termina > pelic[i].empieza - 10)++i;
		siguiente[pelicAux[j].ind] = i;
		++j;
	}
	while (j < N) {
		siguiente[pelicAux[j].ind] = N;
		++j;
	}
	//Coste de calcular el siguiente de cada pelicula: O(N) ent tiempo y O(1) en espacio adicional
	
	std::vector<int> maxTiempo(N + 1);
	maxTiempo[N] = 0;
	for (int i = N - 1; i >= 0; --i) {
		maxTiempo[i] = std::max(pelic[i].dur + maxTiempo[siguiente[i]], maxTiempo[i + 1]);
	}
	//Coste de rellenar la tabla : O(N) en tiempo y O(1) en espacio adicional

	std::cout << maxTiempo[0] << '\n';
}
//Coste total: O(NlogN) en tiempo y O(N) en espacio adicional



bool resuelveCaso() {
	int N;
	std::cin >> N;
	if (N == 0)return false;

	std::vector<pelic> pelics(N); char basura;
	int hora, min;
	for (int i = 0; i < N; ++i) {
		std::cin >> hora >>basura>> min >> pelics[i].dur;
		pelics[i].empieza = 60 * hora + min;
	}

	std::sort(pelics.begin(), pelics.end());
	resolver(pelics);


	return true;
}

int main() {
	std::ios_base::sync_with_stdio(false);
	while (resuelveCaso());
}