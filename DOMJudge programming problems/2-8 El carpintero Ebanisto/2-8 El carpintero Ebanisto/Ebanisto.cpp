//Marcos Herrero

#include <iostream>
#include <vector>

/*
Def:
minEsfuerzo(i,j) es el minimo esfuerzo que debe hacer Ebanisto para realizar todos los cortes entre c[i-1] y c[i+j] (c[-1]= 0, es el inicio del tronco y c[N] = L el final)
0 <= i <= N, 0<= j <= N-i

minEsfuerzo(i,0) = 0;
minEsfuerzo(N-1,j) = 0;

minEsfuerzo(i,j) = min: 0<= k <= j-1: 2*len[i][j] + minEsfuerzo(i, k)+ minEsfuerzo(i+k+1, j-k-1) 

*/

void resolver(int L, std::vector<int> const & c) {
	int N = c.size();

	std::vector<std::vector<int>> len(N+1); // distancia entre c[i-1] y c[i+j]

	for (int j = 0; j < N; ++j)len[0].push_back(c[j]);
	len[0].push_back(L);

	for (int i = 1; i < N; ++i) {
		int suma = c[i] - c[i - 1];

		for (int j = 0; j < N-i-1; ++j) {
			len[i].push_back(suma);
			suma += (c[i + j+1 ]-c[i+j]);
		}

		len[i].push_back(suma);
		len[i].push_back(suma + L - c[N - 1]);
	}

	len[N].push_back(L-c[N-1]);

	std::vector<std::vector<int>> minEsfuerzo(N+1, std::vector<int>(N+1));

	for (int i = 0; i <= N; ++i) minEsfuerzo[i][0] = 0;


	for (int i = N; i >= 0; --i) {
		for (int j = 1; j <= N-i; ++j) {
			int min = minEsfuerzo[i+1][j-1];
			for (int k = 1; k <= j - 1; ++k) {
				if (minEsfuerzo[i][k] + minEsfuerzo[i + k + 1][j - k - 1] < min) min =minEsfuerzo[i][k]+ minEsfuerzo[i + k + 1][j - k - 1];
			}

			minEsfuerzo[i][j] = 2 * len[i][j] + min;
		}
	}

	std::cout << minEsfuerzo[0][N] << '\n';
}

bool resuelveCaso() {
	int L, N;
	std::cin >> L >> N;
	if (L == 0 && N == 0)return false;

	std::vector<int> c(N);
	for (int i = 0; i < N; ++i) std::cin >> c[i];

	resolver(L, c);

	return true;
}

int main() {
	while (resuelveCaso());
}