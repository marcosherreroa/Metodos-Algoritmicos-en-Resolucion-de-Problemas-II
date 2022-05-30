//Marcos Herrero

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

/*
N es la longitud de la cadena c

Def recursiva:
maxLen(i, j) es la maxima longitud de una subsecuencia palindroma en en la subcadena formada por los caracteres i,...,i+j-1 (que empieza en i y tiene long j)
0<= i <= N-1, 0<= j <= N-i

Casos base:
maxLen(i,0) = 0 para todo i
maxLen(i,1) = 1 para todo i

Casos recursivos:
maxLen(i,j) = 2+ maxLen(i+1,j-2) si c[i]= c[i+j-1]
maxLen(i,j) = max( maxLen(i+1, j-1), maxLen(i, j-1)) si c[i] != c[i+j-1]

*/


int lenSubsecPal(std::string const& palabra, int ini, int fin, std::vector<std::vector<int>> & mat) {
	if (ini > fin) mat[ini][fin] = 0;

	else if (ini == fin) mat[ini][fin] = 1;

	else if (palabra[ini] == palabra[fin]) {
		if (mat[ini + 1][fin - 1] == -1)mat[ini + 1][fin - 1] = lenSubsecPal(palabra, ini + 1, fin - 1, mat);
		mat[ini][fin] = mat[ini + 1][fin - 1] + 2;
	}

	else {
		if (mat[ini + 1][fin] == -1)mat[ini + 1][fin] = lenSubsecPal(palabra, ini + 1, fin, mat);
		if (mat[ini][fin - 1] == -1)mat[ini][fin - 1] = lenSubsecPal(palabra, ini, fin - 1, mat);

		mat[ini][fin] = std::max(mat[ini + 1][fin], mat[ini][fin - 1]);
	}

	return mat[ini][fin];
}

void resolver(std::string const & c) {
	int N = c.size();

	std::vector<std::vector<int>> maxLen(N);

	for (int i = 0; i < N; ++i) {
		maxLen[i].push_back(0);
		maxLen[i].push_back(1);
	}
	

	for (int j = 2; j <= N; ++j) {
		for (int i = 0; i <= N - j; ++i) {
			if (c[i] == c[i + j - 1]) maxLen[i].push_back(2 + maxLen[i + 1][j - 2]);
			else maxLen[i].push_back(std::max(maxLen[i + 1][j - 1], maxLen[i][j - 1]));
		}
	}
	//Coste rellenar la tabla : O(N^2) en tiempo y O(1) en espacio adicional

	std::string sec;
	int i = 0, j = N;
	while (j > 0) {
		if (c[i] == c[i + j - 1]) {
			sec.push_back(c[i]);
			++i;
			j -= 2;
		}
		else {
			if (maxLen[i + 1][j - 1] >= maxLen[i][j - 1])++i;
			--j;
		}
	}

	int ind = maxLen[0][N] / 2 - 1;
	while (ind >= 0) {
		sec.push_back(sec[ind]);
		--ind;
	}

	//Coste de la reconstruccion:  O(N) en tiempo y O(1) en espacio adicional

	std::cout << sec << '\n';
}
//Coste total: O(N^2) en tiempo y O(N^2) en espacio adicional

bool resuelveCaso() {
	std::string c;
	std::cin >> c;
	if (!std::cin)return false;

	resolver(c);

	return true;
}

int main() {
	std::ios_base::sync_with_stdio(false);
	while (resuelveCaso());
}