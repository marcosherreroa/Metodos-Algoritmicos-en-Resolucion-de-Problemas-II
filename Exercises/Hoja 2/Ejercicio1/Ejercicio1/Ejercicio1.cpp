//Marcos Herrero

#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>


/*
Numeramos los programas de 0 a n-1. Los consideramos ordenados de menor a mayor peso

Restricciones explicitas:
Solucion de la forma (x[0],...,x[n-1]) donde x[i] pertenece a {0,1} para todo i
Para cada i, x[i] = 1  significa que se coge el programa P_i y x[i]= 0 que no se coge

Restricciones implicitas:
Para que (x[0],...,x[n-1]) sea una solucion valida se ha de cumplir 
(sum: 0<= i <= n-1: x_i*s_i)  <= D


*/

struct Nodo {
	std::vector<bool> sol;
	int i;// se ha completado hasta x[i-1]. Es solucion completa si i = n
	int ocupado; // espacio ocupado por los x[0],...x[i-1] tomados
	int optimista; // cota superior de la mejor solucion alcanzable

	bool operator<(Nodo const & that) const{
		return optimista < that.optimista;
	}
};

/*
Cota optimista: 
Se suman los kilobytes de los programas de i a n-1 que cuyo tamaño es menor que el espacio libre D-ocupado, y nos quedamos con
el minimo entre esta suma y el maximo espacio D que se puede ocupar

Cota pesimista:
Se suman los kilobytes de los programas de i a n-1 que caben en el espacio libre mientras quepan ( para ello es útil tener los programas ordenados de menor a mayor peso)


Podas adicionales:
- Si se encuentra una solucion que ocupa todo el espacio D del disco, no se sigue buscando, pues es imposible encontrar una mejor
*/

int cotaOptimista(int D, std::vector<int> const & s, int ind, int ocupado) { // Coste en O(n) en tiempo y O(1) en espacio adicional
	int n = s.size();
	int cota = ocupado;

	for (int i = ind; cota < D && i < n; ++i) {
		if(s[i] <= D- ocupado) cota += s[i];
	}
	
	if (cota > D) cota = D;

	return cota;
}

int cotaPesimista(int D, std::vector<int> const & s, int ind, int ocupado) {// Coste en O(n) en tiempo y O(1) en espacio adicional
	int n = s.size();
	int cota = ocupado;

	while (ind < n && cota + s[ind] <= D) {
		cota += s[ind];
		++ind;
	}

	return cota;
}

void resolver(int D, std::vector<int> const & s) {// se asume que s llega ordenado de menor a mayor
	int n = s.size();

	std::priority_queue<Nodo> pq;
	std::vector<bool> solMejor(n, false);
	int maxEspacio = cotaPesimista(D,s,0,0);
	bool parar = false;
	int pesimista;

	Nodo x = { std::vector<bool>(n),0,0,D };
	pq.push(x);

	while (!parar && !pq.empty() && pq.top().optimista >= maxEspacio) {
		x = pq.top();
		pq.pop();

		++x.i;

		//Hijo 1: se almacena el programa i
		if (s[x.i - 1] <= D - x.ocupado) {
			x.sol[x.i - 1] = true;
			x.ocupado += s[x.i - 1];

			if (x.i == n) {
				if (x.ocupado >= maxEspacio) {
					maxEspacio = x.ocupado;
					solMejor = x.sol;
					if (maxEspacio == D)parar = true;
				}
			}
			else {
				x.optimista = cotaOptimista(D, s, x.i, x.ocupado);

				if (x.optimista >= maxEspacio) {
					pq.push(x);

					//la cota pesimista coincide con la del padre
				}
			}

			x.ocupado -= s[x.i - 1];
		}

		//Hijo 2: no se almacena el programa i
	
		x.sol[x.i - 1] = false;

		if (x.i == n) {
			if (x.ocupado >= maxEspacio) {
				maxEspacio = x.ocupado;
				solMejor = x.sol;
				if (maxEspacio == D)parar = true;
			}
		}
		else {
			x.optimista = cotaOptimista(D, s, x.i, x.ocupado);

			if (x.optimista >= maxEspacio) {
				pq.push(x);

				pesimista = cotaPesimista(D, s, x.i, x.ocupado);
				if (pesimista > maxEspacio)maxEspacio = pesimista;
			}
		}
	}

	std::cout << maxEspacio << '\n';
	for (int i = 0; i < n; ++i) {
		if (solMejor[i])std::cout << i << ' ';
	}

	std::cout << '\n';
}
/*
Coste en el caso peor en O(2^n * n) , el tamaño del espacio de soluciones por el coste de las cotas en cada iteracion
Coste en espacio adicional en O(n^2) (n nodos simultaneos en la cola, y cada uno almacena un vector de tamaño n)

*/

bool resuelveCaso() {
	int n, D;
	std::cin >> n >> D;
	if (!std::cin)return false;

	std::vector<int> s(n);
	for (int i = 0; i < n; ++i)std::cin >> s[i];

	resolver(D, s);
	return true;
}

int main() {
	while (resuelveCaso());
}