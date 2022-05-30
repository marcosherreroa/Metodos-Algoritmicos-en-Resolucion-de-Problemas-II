//Marcos Herrero

#include <iostream>
#include <vector>
#include <algorithm>

/*
Los cubos se numeran de 0 a N-1

Def recursiva:
maxComer(i,j) es lo maximo que puede comer considerando los cubos i, i+1,... i+2j-1 (j es la mitad de la longitud, pues sabemos que esta es par)
0<= i <= N-1,0<= j <= (N-i+1)/2

Casos base:
maxComer(i,0)= 0 para todo i

Casos recursivos:


Definimos:

s1(i,j) es el maximo que puede comer si elige el cubo de la izquierda

  s1(i,j) = cubos[i] + maxComer[i+2][j-1] si cubos[i+1] > cubos[i+j-1] ( la otra vaca elige el de la izquierda)
  s1(i,j) = cubos[i] + maxComer[i+1][j-1] en caso contrario  (la otra vaca elige el de la derecha)

s2(i,j) es el maximo que puede comer si elige el cubo de la derecha

  s2(i,j) = cubos[i+j-1] + maxComer[i+1][j-1] si cubos[i] > cubos[i+j-2]
  s2(i,j) = cubos[i+j-1] + maxComer[i][j-1] si en caso contrario)

Entonces:
maxComer(i,j) = max(s1(i,j),s2(i,j));

*/

void resolver(std::vector<int> const & cubos) {
	int N = cubos.size();

	std::vector<std::vector<int>> tabla(N, std::vector<int>(N / 2 + 1, 0));// Espacio adicional: O(N^2/2)

	if (N % 2 == 0)for (int i = 0; i < N; ++i)tabla[i][0] = 0;
	else for (int i = 0; i < N; ++i)tabla[i][0] = cubos[i];

	if (N % 2 == 0) {
		for (int j = 2; j < N + 1; j += 2) {
			for (int i = 0; i <= N - j; ++i) {
				int s1;
				if (cubos[i + 1] > cubos[i + j - 1])s1 = cubos[i] + tabla[i + 2][j / 2 - 1];
				else s1 = cubos[i] + tabla[i + 1][j / 2 - 1];

				int s2;
				if (cubos[i] > cubos[i + j - 2])s2 = cubos[i + j - 1] + tabla[i + 1][j / 2 - 1];
				else s2 = cubos[i + j - 1] + tabla[i][j / 2 - 1];

				tabla[i][j / 2] = std::max(s1, s2);
			}
		}

	}

	else {
		for (int j = 3; j < N + 1; j += 2) {
			for (int i = 0; i <= N - j; ++i) {
				int s1;
				if (cubos[i + 1] > cubos[i + j - 1])s1 = cubos[i] + tabla[i + 2][(j - 1) / 2 - 1];
				else s1 = cubos[i] + tabla[i + 1][(j - 1) / 2 - 1];

				int s2;
				if (cubos[i] > cubos[i + j - 2])s2 = cubos[i + j - 1] + tabla[i + 1][(j - 1) / 2 - 1];
				else s2 = cubos[i + j - 1] + tabla[i][(j - 1) / 2 - 1];

				tabla[i][(j - 1) / 2] = std::max(s1, s2);
			}
		}

	}

	std::cout << tabla[0][N / 2] << '\n';
}

bool resuelveCaso() {
	int N;
	std::cin >> N;
	if (N == 0)return false;

	std::vector<int> cubos(N);
	for (int i = 0; i < N; ++i) std::cin >> cubos[i];

	resolver(cubos);
	return true;
}

int main() {
	std::ios_base::sync_with_stdio(false);
	while (resuelveCaso());
}