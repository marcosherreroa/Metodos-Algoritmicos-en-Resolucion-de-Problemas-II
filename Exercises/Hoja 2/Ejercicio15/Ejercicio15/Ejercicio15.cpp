//Marcos Herrero

#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

/*
Supermercados numerados de 0 a m-1 
Productos numerados de 0 a n-1

Restricciones explicitas:
Las soluciones son tuplas de la forma (s[0],...,s[n-1]) donde s[i] pertenece a {0,...,m-1}para todo 0<= i <= n-1
s[i]= k pertenciente a {0,...,m-1} representa que Alonso compra el producto i en el supermercado k. Esta representacion
de la solucion ya obliga a que Alonso compre cada producto en exactamente un supermercado( no deje productos sin comprar y no compre un mismo
producto varias veces), como exige el problema

Restricciones implicitas:
Alonso no quiere comprar en  cada supermercado mas de tres productos , asi que la restriccion implicita que han de verificar las tuplas
soluciones validas es que en cada tupla no aparezca mas de tres veces el mismo valor de 0 a m-1. Esto lo controlaremos mediante un vector auxiliar
supermercados[0...m-1] que lleve la cuenta dekl numero de productos adquiridos en cada supermercado

El objetivo es minimizar el coste total de la compra, sin violar ninguna restriccion

*/

struct Nodo {
	std::vector<int> s;//solucion parcial
	int i; // se ha completado hasta i-1. Es solucion completa si y solo si i = n
	double acum; // precio total de los productos adquiridos en la solucion parcial
	std::vector<int> supermercados; //numero de productos comprados en cada supermercado. Para satisfacer la restriccion
	                                 // implicita, ha de ser supermercados[i] <= 3 para todo i
	double optimista; // cota inferior del minimo precio que a Alonso le costara la compra

	bool operator<(Nodo const & that)const {//mayor prioridad el que tenga menor cota optimista
		return optimista > that.optimista;
	}
};

/*
Cotas optimistas:
1. Aproximar el precio de lo que queda por comprar por 0. 
   Coste en cada llamada : O(1). No hay precalculo ni espacio adicional

2. Aproximar el precio de lo que queda por comprar por el minimo de la matriz precios multiplicado por el numero de productos que quedan por adiquirir
   Coste en cada llamada: O(1). Coste de precómputo: O(m*n). Coste en espacio adicional: O(1)

3. Aproximar el precio de cada producto que queda por comprar por el minimo precio de ese producto. Como los productos los vamos comprando
   en orden, es util llevar precalculado un vector de sumas parciales de los minimos precios
   sumMinPrecio[i] = sum: i<= k1 <= n-1 : (min: 0<= k2 <= m-1: precios[k2][k1])   para todo 0<= i <= n-1
   Coste en cada llamada: O(1). Coste de preçomputo: O(m*n) . Coste en espacio adicional: O(n)

 4. Aproximar el precio de cada producto que queda por comprar por el minimo precio de ese producto en un supermercado que no este
    saturado en la solucion parcial
	Coste en cada llamada: O(n*m). No hay precomputo ni espacio adicional

Elegimos la cota 3

Cotas pesimistas:
1. Completar la solucion parcial a una solucion valida, comprando cada producto en el primer supermercado no saturado
   Coste en cada llamada: O(n+m) . No hay precalculo ni espacio adicional

2. Aproximar el precio de lo que queda por comprar por el maximo de la matriz de precios multiplicado por el numero de productos que quedan por adquirir
   Coste en cada llamada: O(1). Coste de precómputo: O(m*n). Coste en espacio adicional: O(1)

3. Aproximar el precio de cada producto que queda por comprar por el maximo precio de ese producto. Como los productos los vamos comprando en orden,
  es util llevar precalculado un vector de suams parciales de los maximos precios
  sumMaxPrecio[i] = sum: i<= k1 <= n-1 : (min: 0<= k2 <= m-1: precios[k2][k1])   para todo 0<= i <= n-1
  Coste en cada llamada: O(1). Coste de precomputo: O(m*n). Coste en espacio adicional: O(n)

4. Aproximar el precio de cada producto que queda por comprar por el maximo precio de ese producto en un supermercado que no este
   saturado en la solucion parcial
   Coste en cada llamada: O(n*m). No hay precomputo ni espacio adicional

(para el futuro, investigar si es viable la cota de Victor
Elegimos la cota 3
      
*/

double cotaOptimista(int i, double acum, std::vector<double> const & sumMinPrecio) {//Coste O(1)
	return acum + sumMinPrecio[i];
}

double cotaPesimista(int i, double acum, std::vector<double> const & sumMaxPrecio) {//Coste O(1)
	return acum + sumMaxPrecio[i];
}

void resolver(std::vector<std::vector<double>> const & precios) {
	int m = precios.size(), n = precios[0].size();

	std::vector<double> sumMinPrecio(n), sumMaxPrecio(n);
	int sumaMin = 0, sumaMax = 0; int max, min;
	for (int j = n - 1; j >= 0; --j) {
		min = precios[0][j];
		max = precios[0][j];

		for (int i = 1; i < m; ++i) {
			if (precios[i][j] < min)min = precios[i][j];
			if (precios[i][j] > max) max = precios[i][j];
		}

		sumaMin += min;
		sumaMax += max;
		sumMinPrecio[j] = sumaMin;
		sumMaxPrecio[j] = sumaMax;
	}

	std::priority_queue<Nodo> pq;
	std::vector<int> sMejor;
	double minPrecio = cotaPesimista(0, 0, sumMaxPrecio);

	Nodo x = { std::vector<int>(n), 0, 0, std::vector<int>(m,0), cotaOptimista(0,0,sumMinPrecio) };
	pq.push(x);

	while (!pq.empty() && pq.top().optimista <= minPrecio) {
		x = pq.top();
		pq.pop();

		++x.i;
		for (int k = 0; k < m; ++k) {
			if (x.supermercados[k] < 3) {
				x.s[x.i - 1] = k;
				x.acum += precios[k][x.i - 1];
				++x.supermercados[k];

				if (x.i == n) {
					if (x.acum <= minPrecio) {
						sMejor = x.s;
						minPrecio = x.acum;
					}
				}
				else {
					x.optimista = cotaOptimista(x.i, x.acum, sumMinPrecio);

					if (x.optimista <= minPrecio) {
						pq.push(x);

						minPrecio = std::min(minPrecio, cotaPesimista(x.i, x.acum, sumMaxPrecio));
					}
				}

				x.acum -= precios[k][x.i - 1];
				--x.supermercados[k];
			}
		}
	}

	std::cout << "El minimo precio es " << minPrecio << " y corresponde a la siguiente asignacion:\n";
	for (int j = 0; j < n; ++j) {
		std::cout << "Producto " << j << " :  Supermercado " << sMejor[j]<<'\n';
	}
}

bool resuelveCaso(){
	int n, m;
	std::cin >> n >> m;
	if (!std::cin)return false;

	std::vector<std::vector<double>> precios(m, std::vector<double>(n));
	for (int i = 0; i < m; ++i) {
		for (int j = 0; j < n; ++j) {
			std::cin >> precios[i][j];
		}
	}

	resolver(precios);
	return true;
}

int main() {
	while (resuelveCaso());
}