//Marcos Herrero

#include <iostream>
#include <vector>
#include <algorithm>
#include <stack>
#include <iomanip>

/*
Renumeramos las x[i] de 0 a n-1

* maxCociente:
Definicion recursiva:
maxCociente(i,j) es el maximo cociente que se puede obtener al colocar parentesis en la expresion x[i]/x[i+1]/.../x[i+j-1]
0<= i <= n-1, 0<= j<= n-i

Casos base:
maxCociente(i,0) = 1 para todo 0<=i <= n-1
maxCociente(i,1) = x[i] para todo 0<= i <= n-1

Casos recursivos:
maxCociente(i,j) = max: 1 <= k <= j-1 : maxCociente(i,k)/minCociente(i+k,j-k)   
                  si j >=2 , 0<= i <= n-j


*minCociente:
Def recursiva:
minCociente(i,j) es el minimo cociente que se puede obtener al colocar parentesis en la expresion x[i]/x[i+1]/.../x[i+j-1]
0<= i <= n-1, 0<= j<= n-i

Casos base:
minCociente(i,0) = 1 para todo 0<=i <= n-1
minCociente(i,1) = x[i] para todo 0<= i <= n-1

Casos recursivos:
minCociente(i,j) = max: 1 <= k <= j-1 : minCociente(i,k)/maxCociente(i+k,j-k)
                   si j >=2 , 0<= i <= n-j
*/

struct Nodo {
	int j;
	int parentesisAbrir;
	int parentesisCerrar;
	bool max;
};

void imprimirMax(int i, int j, std::vector<double> const & x, std::vector<std::vector<int>> const & kMax, std::vector<std::vector<int>> const & kMin) {
	if (j == 1)std::cout << x[i];
	else {
		std::cout << '(';
		imprimirMax(i + 1, kMax[i][j], x, kMax, kMin);
		std::cout << ")/";
		imprimirMin(i + kMax[i][j], j - kMax[i][j], x, kMax, kMin);
		std::cout << ')';
	}
}

//para evitar sacar cosas habria que discutirlo con el valor de kMax. pereza

void resolver(std::vector<double> const & x) {
	int n = x.size();

	std::vector<std::vector<double>> maxCociente(n, std::vector<double>(n + 1));
	std::vector<std::vector<double>> minCociente(n, std::vector<double>(n + 1));
	std::vector<std::vector<int>> kMax(n, std::vector<int>(n + 1));
	std::vector<std::vector<int>> kMin(n, std::vector<int>(n + 1));

	for (int i = 0; i < n; ++i) {
		maxCociente[i][0] = 1;
		maxCociente[i][1] = x[i];
		minCociente[i][0] = 1;
		minCociente[i][1] = x[i];
	}
	
	for (int j = 2; j <= n; ++j) {
		for (int i = 0; i <= n - j; ++i) {
			maxCociente[i][j] = maxCociente[i][1] / minCociente[i + 1][j - 1];
			kMax[i][j] = 1;
			minCociente[i][j] = minCociente[i][1] / maxCociente[i + 1][j - 1];
			kMin[i][j] = 1;
			for (int k = 2; k <= j - 1; ++k) {
				if (maxCociente[i][k] / minCociente[i + k][j - k] > maxCociente[i][j]) {
					maxCociente[i][j] = maxCociente[i][k] / minCociente[i + k][j - k];
					kMax[i][j] = k;
				}

				if (minCociente[i][k] / maxCociente[i + k][j - k] < minCociente[i][j]) {
					minCociente[i][j] = minCociente[i][k] / maxCociente[i + k][j - k];
					kMin[i][j] = k;
				}
			}
		}
	}

	std::cout << "El maximo valor que se puede obtener es " << maxCociente[0][n] << '\n';
	std::cout << "La colocacion de parentesis es: ";
	
	//imprimir con pila

	int i = 0; Nodo aux;
	std::stack<Nodo> st;
	st.push({ n,0, 0, true });
	while (!st.empty()) {
		aux = st.top();
		st.pop();

		if (aux.j == 1) {
			std::cout<<std::setfill('(') << std::setw(aux.parentesisAbrir) << "";
			std::cout << x[i];
			std::cout << std::setfill(')') << std::setw(aux.parentesisCerrar) << "";
			if (i != n - 1)std::cout << '/';
			++i;
		}

		else if (aux.max) {
			st.push({ aux.j - kMax[i][aux.j], 1, aux.parentesisCerrar + 1, false });
			st.push({ kMax[i][aux.j], aux.parentesisAbrir + 1, 1, true });
		}

		else {
			st.push({ aux.j - kMin[i][aux.j], 1, aux.parentesisCerrar + 1, true });
			st.push({ kMin[i][aux.j], aux.parentesisAbrir + 1, 1, false });
		}

	}

	std::cout << '\n';

	std::cout << imprimirMax(0, n) << '\n';
}

bool resuelveCaso() {
	int n;
	std::cin >> n;
	if (n == 0) return false;

	std::vector<double> x(n);
	for (int i = 0; i < n; ++i)std::cin >> x[i];

	resolver(x);
	return true;
}

int main() {
	while (resuelveCaso());
}