//Marcos Herrero

#include <iostream>
#include <vector>
#include <algorithm>

/*
maxVeces(i,j) es el numero maximo de veces que se puede cortar considerando los trozos i,..., i+2j-1

maxVeces(i,0) = 0 para todo i

maxVeces(i,j) = 1 + max( maxVeces(i+1,j

//Preguntar a CarlosT sobre si es posible hacerlo en espacio lineal
*/

void resolver(std::vector<char> const & trozo) {
	int N = trozo.size();

	std::vector<std::vector<int>> maxVeces(N+1, std::vector<int>(N/2+1));

	for (int i = 0; i <= N; ++i)maxVeces[i][0] = 0;

	for (int j = 1; j <= N; ++j) {
		for (int i = 0; i <= N && i+2*j-1 < N; ++i) {
			int s1 = 0;
			if ((trozo[i] == '1' && trozo[i + 2*j - 1] == '1') || (trozo[i] == '2' && trozo[i + 2 * j - 1] == '2'))s1 = 1+ maxVeces[i + 1][j - 1];
			else s1 = maxVeces[i + 1][j - 1];

			int s2 = 0;
			if ((trozo[i] == '1' && trozo[i + 1] == '1') || (trozo[i] == '2' && trozo[i + 1] == '2'))s2 = 1+ maxVeces[i + 2][j - 1];
			else s2 = maxVeces[i + 2][j - 1];

			int s3 = 0;
			if((trozo[i+2*j-2] == '1' && trozo[i + 2*j-1] == '1') || (trozo[i+2*j-2] == '2' && trozo[i + 2*j-1] == '2'))s2 = 1+ maxVeces[i][j - 1];
			else s3 = maxVeces[i][j - 1];

			maxVeces[i][j] = std::max(s1, std::max(s2, s3));
		}
	}

	std::cout << maxVeces[0][N/2]<<'\n';
}


bool resuelveCaso() {
	int N;
	std::cin >> N;
	if (!std::cin)return false;
	std::vector<char> trozo(N);
	for (int i = 0; i < N; ++i)std::cin >> trozo[i];

	resolver(trozo);

	return true;
}

int main() {
	while (resuelveCaso());
}