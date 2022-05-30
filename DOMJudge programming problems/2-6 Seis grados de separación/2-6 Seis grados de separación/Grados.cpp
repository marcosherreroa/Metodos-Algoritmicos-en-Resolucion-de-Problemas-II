//Marcos Herrero

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <unordered_map>

/*
Def:
maxGrado(i,j) es la longitud del mayor grado de entre los  

grado(i,j, k) es la longitud del menor camino de i a j usando como vertices intermedios de 0 a k-1
0<= i <= P-1,0<= j <= P-1, 0<= k <= P




grado(i,j,k) = 1 si hay arista entre i y j
grado(i,j, 0)= inf si no hay arista entre i y j

grado(i,j,k) = min ( grado(i,k-1,k-1)+grado(k-1,j,k-1), grad(i,j,k-1));

*/

void resolver(std::vector<std::vector<int>> & grado) {
	int P = grado.size();

	for (int i = 0; i < P; ++i) grado[i][i] = 0;

	for (int k = 1; k <= P; ++k) { 
		for (int i = P-1; i >= 0; --i) {
			for (int j = P-1; j >= 0; --j) {
				if (grado[i][k - 1] != -1 && grado[k - 1][j] != -1 && grado[i][j] != -1)grado[i][j] = std::min(grado[i][k - 1] + grado[k - 1][j], grado[i][j]);
				else if (grado[i][k - 1] != -1 && grado[k - 1][j] != -1)grado[i][j] = grado[i][k - 1] + grado[k - 1][j];
			}
		}
	}

	int maximo = grado[0][0]; bool infinito = false;
	for (int i = 0; !infinito && i < P; ++i) {  //coste P^2
		for (int j = 0; !infinito && j < P; ++j) {
			if (grado[i][j] == -1)infinito = true;
			else if (grado[i][j] > maximo)maximo = grado[i][j];
		}
	}

	if (infinito)std::cout << "DESCONECTADA\n";
	else std::cout << maximo << '\n';
}

//Coste en tiempo en O(P^3) y en espacio adicional O(P^2)

bool resuelveCaso() {
	int P, R;
	std::cin >> P >> R;
	if (!std::cin)return false;

	if (P == 1)std::cout << 0 << '\n';
	else {

		std::vector<std::vector<int>> grados(P, std::vector<int>(P, -1));
		std::unordered_map<std::string, int> nombreNum;

		std::string p1, p2;
		int cont = 0;
		for (int i = 0; i < R; ++i) {
			std::cin >> p1 >> p2;
			if (p1 != p2) {
				if (nombreNum.count(p1) == 0) {
					nombreNum[p1] = cont;
					++cont;
				}
				if (nombreNum.count(p2) == 0) {
					nombreNum[p2] = cont;
					++cont;
				}

				grados[nombreNum[p1]][nombreNum[p2]] = 1;
				grados[nombreNum[p2]][nombreNum[p1]] = 1;
			}
		}

		if (cont < P)std::cout << "DESCONECTADA\n";
		else resolver(grados);

	}

    return true; 
}

int main() {
	while (resuelveCaso());
	return 0;
}