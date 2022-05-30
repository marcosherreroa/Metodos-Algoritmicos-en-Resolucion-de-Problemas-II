//Marcos Herrero

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <list>

/*
Def:
minLetras(i,j) es minimo numero de letras que hay que añadir entre los caracteres i,...,i+j-1 para que esta subcadena sea palindromo
0<= i <= n-1, 0<= j <=n ( solo tiene sentido i+j-1 < n

minLetras(i,0) = 0 para todo 0<= i <=n-1
minLetras(i,1) = 0;

minLetras(i,j) = minLetras(i+1,j-2) si palabra[i] == palabra[i+j-1]
minLetras(i,j) = 1 +  min (minLetras(i+1,j-1), minLetras(i, j-1)) si palabra[i] != palabra[i+j-1]

*/


void resolver(std::string const & palabra) {
	int n = palabra.size();

	std::vector<std::vector<int>> minLetras(n, std::vector<int>(n + 1,0));

	for (int i = 0; i <= n - 1; ++i) {
		minLetras[i][0] = 0;
		minLetras[i][1] = 0;
	}

	for (int j = 2; j <= n; ++j) {
		for (int i = 0; i < n - j + 1; ++i) {
			if (palabra[i] == palabra[i + j - 1]) minLetras[i][j] = minLetras[i + 1][j - 2];
			else minLetras[i][j] = 1 + std::min(minLetras[i + 1][j - 1], minLetras[i][j - 1]);
		}
	}

	int i = 0, j = n;
	std::list<char> sol; auto itini = sol.begin(), itfin = sol.begin();
	while (j > 1) {
		if (palabra[i] == palabra[i + j - 1]) {
			itini = sol.insert(itini, palabra[i]);
			itfin= sol.insert(itfin, palabra[i+j-1]);
			++itini;
		
			++i;
			j -= 2;
		}

		else {
			if (minLetras[i + 1][j - 1] <= minLetras[i][j - 1]) {
				itini = sol.insert(itini, palabra[i]);
				itfin = sol.insert(itfin, palabra[i]);
				++itini;

				++i;
				--j;
			}
			else {
				itini = sol.insert(itini, palabra[i+j-1]);
				itfin = sol.insert(itfin, palabra[i + j - 1]);
				++itini;

				--j;
			}
		}
	}

	if (j == 1) sol.insert(itini, palabra[i]);
	
	std::cout << minLetras[0][n]<<' ';
	for (auto it = sol.begin(); it != sol.end(); ++it) {
		std::cout << *it;
	}
	std::cout << '\n';
}

bool resuelveCaso() {
	std::string palabra;

	std::cin >> palabra;
	if (!std::cin)return false;

	resolver(palabra);


	return true;
}

int main() {
	std::ios_base::sync_with_stdio(false);
	while (resuelveCaso());
}