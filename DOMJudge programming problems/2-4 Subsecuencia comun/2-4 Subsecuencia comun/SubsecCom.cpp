//Marcos Herrero

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

/*
n es la longitud de X y m la de Y

maxLong(i,j) es la subecuencia comun mas larga entre las subcadenas X[0],...X[i-1] y Y[0],...Y[j-1]
0<= i <= m, 0<= j <= n

maxLong(0,j) = 0 para todo 0<= j <= n
maxLong(i,0) = 0 para todo 0<= i <= m

maxLong(i,j) = 1 + maxlong(i-1,j-1) si X[i-1] = Y[j-1]
maxLong(i,j) = max(maxlong(i-1, j), maxlong(i,j-1)) si X[i-1] != Y[j-1]

*/

void resolver(std::string const & X, std::string const & Y) {
	int m = X.size(), n = Y.size();

	std::vector<std::vector<int>> maxLong(m+1, std::vector<int>(n+1));
	for (int i = 0; i <= m; ++i)maxLong[i][0] = 0;
	for (int j = 0; j <= n; ++j)maxLong[0][j] = 0;

	for (int i = 1; i <= m; ++i) {
		for (int j = 1; j <= n; ++j) {
			if (X[i - 1] == Y[j - 1])maxLong[i][j] = 1 + maxLong[i - 1][j - 1];
			else maxLong[i][j] = std::max(maxLong[i - 1][j], maxLong[i][j - 1]);
		}
	}

	std::string sol;
	for (int i = 0; i < maxLong[m][n]; ++i)sol.push_back(' ');

	int i = m, j = n; int ind = maxLong[m][n] - 1;
	while (i > 0 && j > 0) {
		if (X[i - 1] == Y[j - 1]) {
			sol[ind] = X[i - 1];
			--i;
			--j;
			--ind;
		}
		else {
			if (maxLong[i - 1][j] > maxLong[i][j - 1])--i;
			else --j;
		}
	}

	std::cout << sol<<'\n';
}

bool resuelveCaso() {
	std::string X, Y;
	std::cin >> X >> Y;
	if (!std::cin)return false;

	resolver(X, Y);

	return true;
}

int main() {
	while (resuelveCaso());
}