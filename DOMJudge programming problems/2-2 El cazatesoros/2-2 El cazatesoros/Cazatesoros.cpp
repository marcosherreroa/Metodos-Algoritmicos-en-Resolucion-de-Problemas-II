//Marcos Herrero

#include <iostream>
#include <vector>
#include <algorithm>


struct cofre {
	int prof;
	int oro;
};

/*
p[0...N-1] es el vector de profundidades
o[0...N-1] es el vector de cantidad de oro

Def:
maxOro(i,j) es la cantidad de oro que podra conseguir en j segundos considerando que puede acceder a los cofres de 0 a i-1
0 <= i <=  N, 0<= j <= T

Casos base:
maxOro(0,j) = 0 para todo 0 <= j <= T
maxOro(i,0) = 0 para todo 0<= i <= N

Casos recursivos:
maxOro(i,j) = maxOro(i-1,j) si i >= 1 && j < 3*p[i-1]
maxOro(i,j) = max( o[i-1] + maxOro(i-1,j-3*p[i-1]) , maxOro(i-1, j) ) si i>= 1, j >= 3*p[i-1]

*/

void resolver(int T, std::vector<int> const & p, std::vector<int> const & o) {
	int N = p.size();

	std::vector<std::vector<int>> maxOro(N + 1, std::vector<int>(T + 1));

	for (int j = 0; j <= T; ++j) maxOro[0][j] = 0;
	for (int i = 0; i <= N; ++i)maxOro[i][0] = 0;

	for (int i = 1; i <= N; ++i) {
		for (int j = 0; j <= T && j < 3 * p[i - 1]; ++j) maxOro[i][j] = maxOro[i - 1][j];
		for(int j = 3*p[i-1]; j<= T; ++j)maxOro[i][j] = std::max(o[i - 1] + maxOro[i - 1][j - 3 * p[i - 1]], maxOro[i - 1][j]);
	}

	std::vector<int> indSol;
	int i = N, j = T;
	while (i > 0 && j > 0) {
		if (j >= 3*p[i-1] && o[i - 1] + maxOro[i - 1][j - 3 * p[i - 1]] > maxOro[i - 1][j]) {
			indSol.push_back(i-1);
			j -= 3 * p[i - 1];
		}
		--i;
	}

	std::cout << maxOro[N][T] <<'\n'<< indSol.size()<< '\n';
	for (int i = indSol.size() - 1; i >= 0; --i)std::cout << p[indSol[i]] << ' ' << o[indSol[i]] << '\n';
}

bool resuelveCaso() {
	int T, N;
	std::cin >> T >> N;
	if (!std::cin)return false;

	std::vector<int> p(N), o(N);
	for (int i = 0; i < N; ++i)std::cin >> p[i] >> o[i];

	resolver(T, p, o);

	std::cout << "---\n";
	return true;
}

int main() {
	std::ios_base::sync_with_stdio(false);
	while (resuelveCaso());
}