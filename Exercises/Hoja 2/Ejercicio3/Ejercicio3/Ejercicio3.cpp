//Marcos Herrero

#include <iostream>
#include <vector>
#include <queue>

/*
Renumeramos las mujeres y los hombres de 0 a n-1

Restricciones explicitas:
Solucion de la forma (sol[0],...,sol[n-1]) donde sol[i] es la pareja de la mujer i 

Restricciones implicitas:
No hay, todas la tuplas anteriores son soluciones validas

*/

struct Nodo {
	std::vector<int> sol;
	int i; // se ha completado hasta sol[i-1]. Es solucion completa si i = n
	int acum; // suma de los productos de las preferencias de las parejas ya hechas
	int optimista; // cota superior de la mejor solucion alcanzable
	std::vector<bool> Hmarcas; //Hmarcas[i] = true si y solo si el hombre i ya esta emparejado en la sol actual

	bool operator<(Nodo const & that) const {
		return optimista < that.optimista;
	}
};

/*
Cota optimista:
- Opcion 1 ( la que utilizamos) : Para cada mujer j, llevamos precalculado el mejor producto del que puede formar parte :
     mejor[j] = max: 0<= k <= n: M[j][k]*H[k][j].
  Nuestra cota optimista se obtiene al considerar que todas las mujeres no emparejadas se podrian emparejar de forma que aportaran este mejor producto: 
     optimista = acum + sum:i<= j <= n: mejor[i]
  Su calculo tiene coste en O(n^2) de precomputo inicial y en O(n) en cada nodo. Requiere un coste en espacio adicional en O(n)

  -Opcion 2: Llevamos precalculado el mejor producto de preferencias
       mejor = max: 0<= j <= n && 0 <= k <= n: M[j][k]*H[k][j]
   Nuestra cota optimista se obtiene al considerar que todas las parejas aun no hechas aportan esta cantidad
      optimista = acum + (n - i)mejor
	Su calculo tiene O(n^2) de precomputo inical y O(1) en cada nodo. Requiere un coste en espacio adicional en O(1)

La opcion 1 es mas ajustada que la 2, aunque su calculo tiene un coste mayor

Cota pesimista:
- Opcion 1(la que usamos): La cota pesimista sera un valor obtenido al completar la solucion parcial ( emparejando ordenadamente). Siempre es posible hacer esto porque hay el mismo numero de mujeres que de hombres y no hay restricciones para los emparejamientos
  Su calculo tiene coste en O(n) en cada nodo y ningun coste de precomputo ni en espacio adicional

- Opcion 2: Llevamos precalculado el peor producto de preferencias
    peor = min: 0<= j <= n && 0 <= k <= n: M[j][k]*H[k][j]
  Nuestra cota pesimista se obtiene al considerar que todas las parejas aun no hechas aportan esta cantidad
    pesimista = acum +(n-i)peor
  Su calculo tiene coste O(n^2) de precomputo inicial y O(1) en cada nodo. Requiere un coste en espacio adicional en O(1)

  La opcion 1 es mas ajustada que la 2, aunque su calculo tiene un coste mayor

*/

int cotaOptimista(int i, int acum, std::vector<int> const & mejor) {//Coste en O(n) en tiempo  y O(1) en espacio adicional
	int n = mejor.size();

	while (i < n) {
		acum += mejor[i];
		++i;
	}

	return acum;
}

int cotaPesimista(int i, int acum, std::vector<bool> const & Hmarcas, std::vector<std::vector<int>> const & M, std::vector<std::vector<int>> const & H) {
	//Coste O(n) en tiempo y O(1) en espacio adicional
	int n = M.size();

	int j = 0;
	while (i < n) {
		if (!Hmarcas[j]) {
	        acum += M[i][j] * H[j][i];
			++i;
		}
		++j;
	}

	return acum;
}


void resolver(std::vector<std::vector<int>> const & M, std::vector<std::vector<int>> const & H) {
	int n = M.size();

	std::vector<int> mejor(n, 0);
	for (int i = 0; i < n; ++i) {   // precalculamos mejor. Coste: O(n^2)
		for (int j = 0; j < n; ++j) {
			if (M[i][j] * H[j][i] > mejor[i]) mejor[i] = M[i][j] * H[j][i];
		}
	}

	std::priority_queue<Nodo> pq;
    std::vector<int> solMejor(n);
	int maxAfinidad = cotaPesimista(0, 0, std::vector<bool>(n, false), M, H);
	int pesimista;

	Nodo x = { std::vector<int>(n), 0, 0, cotaOptimista(0,0, mejor), std::vector<bool>(n, false) };
	pq.push(x);

	while (!pq.empty() && pq.top().optimista >= maxAfinidad) {
		x = pq.top();
		pq.pop();

		++x.i;

		for (int k = 0; k < n; ++k) {
			//Hijo k, 0<= k < n: se empareja la mujer i con el hombre k ( toda mujer debe emparejarse para lograr el maximo, por lo que no consideramos el hijo "no emparejar")
			if (!x.Hmarcas[k]) {
				x.sol[x.i-1] = k;
				x.acum += M[x.i-1][k] * H[k][x.i-1];
				x.Hmarcas[k] = true;
				
				if(x.i== n){
					if (x.acum >= maxAfinidad) {
						maxAfinidad = x.acum;
						solMejor = x.sol;
					}
				}

				else {
					x.optimista = cotaOptimista(x.i, x.acum, mejor);

					if (x.optimista >= maxAfinidad) {
						pq.push(x);

						pesimista = cotaPesimista(x.i, x.acum, x.Hmarcas, M, H); // en el hijo 0, la cota pesimista coincide con la del padre y se podria ahorrar
						if (pesimista > maxAfinidad) maxAfinidad = pesimista;
					}
				}

				x.acum -= M[x.i - 1][k] * H[k][x.i - 1];
				x.Hmarcas[k] = false;
			}
		}

	}

	std::cout << maxAfinidad << '\n';
	for (int i = 0; i < n; ++i) {
		std::cout << i << ' ' << solMejor[i] << '\n';
	}

	std::cout << '\n';
}
/*
Coste en el caso peor en O(n!*n) (numero de permutaciones de n elementos por el coste de las cotas)
Coste en espacio adicional en O(n^3) (n^2 nodos simultaneos y cada uno lleva vectores de tamaño n)
*/

bool resuelveCaso() {
	int n;
	std::cin >> n;
	if (!std::cin)return false;

	std::vector<std::vector<int>> M(n, std::vector<int>(n)), H(n, std::vector<int>(n));

	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) {
			std::cin >> M[i][j];
		}
	}

	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) {
			std::cin >> H[i][j];
		}
	}

	resolver(M, H);
	return true;
}

int main() {
	while (resuelveCaso());
}