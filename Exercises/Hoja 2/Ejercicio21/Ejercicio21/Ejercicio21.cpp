//Marcos Herrero

#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

/*
Renumero los costes de 0 a n-1

Restricciones explicitas:
Las soluciones son tuplas de T elementos, (s[0],...,s[T-1]), donde s[i] pertenece a {false,true}={0,1} para todo i entre 0 y T-1
s[i] = true representa que se renueva el movil en el año i, mientras que s[i] = false indica que no se renueva

Restricciones implicitas:
Para que una solucion sea valida, se ha de renovar el movil al menos cada M años, es decir, habra como mucho M-1 false consecutivos en la tupla s

Funcion objetivo:
Se quiere minimizar el coste total de las renovaciones a lo largo de los T años, es decir:
sum: 0<= k <= T-1: s[k]+c[k]


*/

struct Nodo {
	std::vector<bool> s;//solucion parcial
	int i; // se ha rellenado hasta i-1. Es solucion completa si y solo si i = T
	double acum; // precio acumulado de las renovaciones ya contempladas en la solucion parcial
	int renov; //años que podemos pasar sin volver a renovar el movil. Es M-1 en los años que se acaban de renovar y,
	           //cuando es 0, estamos obligados a renovar
	double optimista; // cota inferior del menor coste total en una solucion que satisfaga las restricciones
	bool operator<(Nodo const & that)const { // mas prioridad el que tenga menor cota optimista
		return optimista > that.optimista;
	}
};

/*
Se puede aplicar el esquema optimista-pesimista porque toda solucion parcial es completable a una solucion alcanzable, por ejemplo, renovando el movil
todos los años que quedan

Cota optimista:
-Opcion 1: estimar por 0 el coste de todas las renovaciones venideras, es decir, optimista = acum
Coste en cada iteracion: O(1)  No hay precomputo ni espacio adicional

-Opcion 2: estimar el coste de las renovaciones venideras por el coste minimo (entre c[0], c[1],...,c[T-1]) multiplicado por el numero de años
 que quedan dividido entre M (division entera)( sabemos que si quedan kM años seran necesarias al menos k renovaciones mas, mientras que si quedan kM-1 solo seran
 necesarias k-1). Entonces optimista = acum + (min: 0<= k <= M-1: c[k])*(T-i)/M. Llevariamos precaclulado  el minimo del vector c
 Coste en cada iteracion: O(1);  Coste de precomputo: O(T); Coste en espacio adicional: O(1)

-Opcion 3: estimar el coste de las renovaciones venideras por el coste minimo de las renovaciones en los años que quedan (entre c[i],...,c[T-1]) 
multiplicado por el numero de años que quedan dividido entre M. Entonces optimista = acum + (min: i<= k <= T-1: c[k])*(T-i)/M . Llevariamos
precalculado el minimo entre c[i],c[i+1],...,c[T-1] en un vector minC de tamaño T
Coste en cada iteracion: O(1); Coste de precomputo: O(T); Coste en espacio adicional: O(T)

Elegimos la opcion 3 por ser la cota mas ajustada y , sin embargo, no tener un coste asintotico mayor

Cota optimista:
-Opcion 1: completamos a una solucion renovando el movil en todos los años que quedan
  pesimista = acum + (sum: i<=k<= T: c[k])
  Llevamos precalculadas las sumas parciales (sumas[i] = sum: i<= k <= T: c[k])
  Coste en cada iteracion: O(1); Coste de precomputo: O(T); Coste en espacio adicional: O(T)

-Opcion 2: completamos a una solucion renovando el movil exactamente  cada M años. Podemos llevar precalculadas la suma de los costes en las posiciones
 congruentes con j mod M y mayores que j 
 sumPosModM[j] = sum: 0<= k <= (T-j-1)/M : c[M*k+j]
 pesimista = acum + sumPosMod[i+renov] si i+renov <= T-1
 pesimista = acum si i+renov >= T
 Coste en cada iteracion: O(1); Coste de precomputo: O(T); Coste en espacio adicional: O(T)

 Elegimos la opcion 2 por ser la cota mas ajustada y, sin embargo, no tener un coste asintotico mayor
*/

double cotaOptimista(int i, double acum, int M, std::vector<double> const & minC) {//Coste O(1)
	return acum + minC[i] * ((minC.size() - i) / M);
}

double cotaPesimista(int i, double acum, int renov, std::vector<double> const & sumPosMod) {//Coste O(1)
	if (i + renov < sumPosMod.size()) return acum + sumPosMod[i + renov];
	else return acum ;
}

void resolver(std::vector<double> const & c, int M) {
	int T = c.size();

	std::vector<double> minC(T);
	minC[T - 1] = c[T-1];
	for (int i = T - 2; i >= 0; --i) {
		if (c[i] < minC[i + 1])minC[i] = c[i];
		else minC[i] = minC[i + 1];
	}

	std::vector<double> sumPosMod(T);
	for (int r = 0; r < M; ++r) {
		int j = T - 1 - r, suma = 0;
		while (j >= 0) {
			suma += c[j];
			sumPosMod[j] = suma;
			j -= M;
		}
	}

	std::priority_queue<Nodo> pq;
	std::vector<bool> sMejor;
	double minCoste = cotaPesimista(0, 0,M-1, sumPosMod);

	Nodo x = { std::vector<bool>(T), 0, 0 , M - 1, cotaOptimista(0,0,M,minC)};
	pq.push(x);

	while (!pq.empty() && pq.top().optimista <= minCoste) {
		x = pq.top();
		pq.pop();

		++x.i;

		//Hijo 1: No renovar ( si es posible)
		if (x.renov > 0) {
			x.s[x.i - 1] = false;
			--x.renov;

			if (x.i == T) {
				if (x.acum <= minCoste) {
					sMejor = x.s;
					minCoste = x.acum;
				}
			}

			else {
				x.optimista = cotaOptimista(x.i, x.acum, M, minC);

				if (x.optimista <= minCoste) {
					pq.push(x);
					minCoste = std::min(minCoste, cotaPesimista(x.i, x.acum, x.renov, sumPosMod));
				}
			}

			++x.renov;
		}

		//Hijo 2: Renovar
		x.s[x.i - 1] = true;
		x.acum += c[x.i - 1];
		x.renov = M - 1;

		if (x.i == T) {
			if (x.acum <= minCoste) {
				sMejor = x.s;
				minCoste = x.acum;
			}
		}

		else {
			x.optimista = cotaOptimista(x.i, x.acum, M, minC);

			if (x.optimista <= minCoste) {
				pq.push(x);
				minCoste = std::min(minCoste, cotaPesimista(x.i, x.acum, x.renov, sumPosMod));
			}
		}
	}

	std::cout << "El minimo coste es " << minCoste << '\n';
	std::cout << "Los años en que se renueva son: ";
	for (int i = 0; i < T; ++i) {
		if (sMejor[i]) std::cout << i << " ";
	}
	std::cout << '\n';
}
/*
Coste en tiempo: tamaño del espacio de soluciones + coste del trabajo realizado en cada nodo
Como las cotas se calculan con coste constante, el coste del trabajo realizado en cada nodo es constante
Entonces el coste en tiempo esta en el orden del tamño del espacio de soluciones, O(2^n), en el caso peor

Coste en espacio adicional: espacio ocupado por cado nada + numeor de nodos simultaneamente en la cola
Cada nodo ocupa un tamaña en O(n) (contiene un vector de n elementos)
El numero de nodos simultaneos en la cola  es del orden de la altura del arbol de exploracion por el numero de hijos generados en cada nodo
Es decir esta en O(2*n) = O(n)
Por tanto, el coste en espacio adicional del algoritmo esta en O(n^2) en el caso peor

*/

bool resuelveCaso() {
	int T, M;
	std::cin >> T >> M;
	if (!std::cin)return false;

	std::vector<double> c(T);
	for (int i = 0; i < T;  ++i) std::cin >> c[i];

	resolver(c, M);
	return true;
}

int main() {
	while (resuelveCaso());
}