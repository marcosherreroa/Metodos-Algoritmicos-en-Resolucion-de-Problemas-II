//Marcos Herrero

#include <iostream>
#include <vector>
#include <queue>
#include <list>

/*
Se numeran los comensales de 0 a n-1 y las mesas de 0 a m-1

Restricciones explicitas:
Las soluciones del problema son tuplas de la forma (s[0],...,s[n-1]) donde s[i] es un numero entre 0 y m-1 para todo 0<= i <= n-1
Para cada i, s[i] representa la mesa asignada al comensal i

Restricciones implicitas:
Para que una solucion (s[0], ..., s[n-1]) sea valida cada mesa ha de tener asignados exactamente n/m comensales , es decir;
Para todo 0 <= j <= m , (count: 0<= k<= m-1 : s[k] = j) = n/m

Funcion objetivo:
Queremos maximizar la suma de la satisfacciones de cada mesa, donde la satisfaccion en una mesa es la suma de la afinidades de cada par de comensales de la misma
*/

struct Nodo {
	std::vector<int> s; // solucion parcial: asignacion de comensales a mesas
	int i;//se ha completado hasta s[i-1]. Si i == n, la solucion es completa
	int acum; // satisfaccion acumulada hasta el momento
	int optimista;// cota superior de la mejor suma de satisfacciones
    std::vector<std::list<int>> mesas; // mesas[i] es un lista que contiene a los comensales ya asignados a la mesa i para cada mesa usada
	                                     //( el size de mesas nos da informacion sobre el numero de mesas usadas hasta el momento)

	bool operator<(Nodo const & that)const {// tiene mas prioriodad el que tiene mayor cota optimista
		return optimista < that.optimista;
	}
};

/*
Cota optimista:
(El mejor hueco para cada comensal)
1. Estimar que cada comensal aun no asignado aportara lo maximo que puede aportar ese comensal, es decir, lo que  aportaria al añadirse a una mesa con un solo hueco
   y en el que las afinidades de los comensales ya sentados hacia el nuevo y del nuevo hacia los ya sentados sean maximas
   Para cada comensal i, 0<= i <= n-1, llevamos precalculado el mayor coeficiente de a en la fila i y el mayor coeficiente en la columna i(sin contar la diagonal)
   maxFila[i] = max: 0<= k <= n-1 && k!= i: a[i][k]   ; maxColumna[i] = max:0<= k<= n-1 && k!= i:a[k][i]
   Cada comensal aun no asignado  aporta a la satisfaccion total  como maximo maxFila[i]*(n/m-1)+ maxColumna[i]*(n/m-1), es decir
  optimista = acum +(sum: i<= k <= n-1: (maxFila[i]+maxColumna[i])*(n/m-1)
  Se puede llevar precalculado el segundo sumando para cada i entre 0 y n-1, de forma que el coste de calculo de la cota en cada iteracion seria constante
  Coste en cada llamada: O(1), Coste de precálculo: O(n^2), Coste en espacio adicional: = O(n)

(El mejor comensal para cada hueco)
2.Estimar que, en las asignaciones que restan, todos los comensales aportaran el maximo de la matriz(sin contar la diagonal). Es necesario acceder al recuento de huecos por mesa,
  por lo que esta cota tendra necesariamente coste lineal en m. Denotando M := mesas.size() y M[i]:= mesas[i].size() :
  optimista = acum + (sum: 0<= i <= M-1 && M[i] <= j <= n/m-1: 2*maxMatriz*j) + (m-mesas.size())(sum: 0<= j <= n/m-1: 2*maxmatriz*j)=
            = acum + 2*maxMatriz* ((M * ((n/m-1)*n/m)/2  - sum: 0<= i <= M-1 : (M[i]-1)*M[i])/2 ) + (m-M)* ((n/m-1)*n/m)/2 
  Coste en cada llamada: O(m), Coste de precálculo: O(n^2), Coste en espacio adicional: O(1)

3. Para reducir el coste de la anterior, se podria llevar calculado el numero total de huecos libres en las mesas ya empezadas y aplicar la cota 1 sobre este
   A continuacion, aplicar la cota 2 solo para las mesas vacias, donde si tiene coste constante. 
   Coste en cada llamada: O(1), Coste de precálculo: O(n^2), Coste en espacio adicional: O(n)

He implementado la cota 1

Cota pesimista:( se puede utilizar porque en este problema toda solucion parcial es completable a una solucion)

(para cada comensal el peor hueco)
1. Estimar por 0 la afinidad aportada por los comensales aun no asignados
 Coste en cada llamada: O(1). No hay precálculo ni espacio adicional

(para cada hueco el peor comensal)
2. Estimar que, en las asignaciones que restan, todos los comensales aportaran el minimo de la matriz. Es necesario acceder al recuento de huecos por mesa,
   por lo que esta cota tendra necesariamente coste lineal en m. Denotando M := mesas.size() y M[i]:= mesas[i].size():
   pesimista  = acum + (sum: 0<= i <= M-1 && M[i] <= j <= n/m-1: 2*minMatriz*j) + (m-mesas.size())(sum: 0<= j <= n/m-1: 2*minMatriz*j)=
            = acum + 2*minMatriz* ((M * ((n/m-1)*n/m)/2  - sum: 0<= i <= M-1 : (M[i]-1)*M[i])/2 ) + (m-M)* ((n/m-1)*n/m)/2 
   Coste en cada llamada: O(m), Coste de precálculo: O(n^2), Coste en espacio adicional: O(1)

3. Para reducir el coste de la anterior, se podria estimar por 0 la satisfaccion que aportaran al rellenarse los huecos de las mesas ya empezadas
   A continuacion, aplicar la cota 2 solo para las mesas vacias, donde si tiene coste constante. 
   Coste en cada llamada: O(1), Coste de precálculo: O(n^2), Coste en espacio adicional: O(n)

4. Completar a una solucion asignando los comensales en orden
   Coste en cada llamada: O(n*m) . No hay precálculo ni espacio adicional

5. Partir de  una solucion valida y modificarla en cada paso con coste m, de forma que en cada iteracion sea una completacion valida de la solucion parcial
   Coste en cada llamada: O(m) , Coste de precálculo: O(n*m), Coste en espacio adicional : O(n)

Implemento la cota 3

*/

int cotaOptimista(int i, int acum,int m, std::vector<int> const & sumaParcial) {//Coste O(1)
	return acum+ sumaParcial[i];
}

int cotaPesimista(int i, int acum, int n, int m, int minMatriz, int nmesas) {//Coste O(1)
	return acum+ 2* minMatriz * (m - nmesas)*((n / m - 1)*n / m) / 2;
}

void resolver(int m, std::vector<std::vector<int>> const & a) {
	int n = a.size();
	int p = n / m;

	std::vector<int> maxFila(n, INT_MIN), maxColumna(n, INT_MIN),
		             minFila(n, INT_MAX), minColumna(n, INT_MAX); //Precomputos para las cotas .Coste de calculo O(n^2)
	int minMatriz = INT_MAX;
	
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) {
			if (i != j) {
				if (a[i][j] > maxFila[i])maxFila[i] = a[i][j];
				else if (a[i][j] < minFila[i])minFila[i] = a[i][j];

				if (a[i][j] > maxColumna[j])maxColumna[j] = a[i][j];
				else if (a[i][j] < minColumna[j])minColumna[j] = a[i][j];

				if (a[i][j] < minMatriz) minMatriz = a[i][j];
			}
		}
	}

	std::vector<int> sumaParcial(n); double suma = 0;
	for (int i = n - 1; i >= 0; --i) {
		suma += (maxFila[i] + maxColumna[i])* (n / m - 1);
		sumaParcial[i] = suma;
	}

	std::priority_queue<Nodo> pq;
	std::vector<int> sMejor;
	int maxSat = cotaPesimista(0, 0,n, m, minMatriz, 0);
	int suma,pesimista;


	Nodo x = { std::vector<int>(n),0, 0, cotaOptimista(0,0,m,sumaParcial),std::vector<std::list<int>>() };
	pq.push(x);

	while (!pq.empty() && pq.top().optimista >= maxSat) {
		x = pq.top();
		pq.pop();

		++x.i;
		for (int k = 0; k < x.mesas.size(); ++k) {// lo colocamos en alguna de las mesas ya utilizadas
			if (x.mesas[k].size() < p) {// si hay hueco en la mesa
				suma = 0;
				for (int j: x.mesas[k]) {
					suma += (a[x.i - 1][j] + a[j][x.i - 1]);
				}

				x.s[x.i - 1] = k;
				x.acum += suma;
				x.mesas[k].push_back(x.i - 1);

				if (x.i == n) {
					if (x.acum >= maxSat) {
						sMejor = x.s;
						maxSat = x.acum;
					}
				}

				else {
					x.optimista = cotaOptimista(x.i, x.acum, m, sumaParcial);

					if (x.optimista >= maxSat) {
						pq.push(x);

						pesimista = cotaPesimista(x.i, x.acum, n,m, minMatriz, x.mesas.size());
						if (pesimista > maxSat)maxSat = pesimista;
					}
				}

				x.acum -= suma;
				x.mesas[k].pop_back();
			}
		}

		//Mesa nueva ( haciendolo asi , en lugar de considerar todas la vacias, se evita repetir soluciones)

		if (x.mesas.size() < m) {
			x.s[x.i - 1] = x.mesas.size();
			x.mesas.push_back({ x.i - 1 });

			if (x.i == n) {
				if (x.acum >= maxSat) {
					sMejor = x.s;
					maxSat = x.acum;
				}
			}

			else {
				x.optimista = cotaOptimista(x.i, x.acum, m,sumaParcial);

				if (x.optimista >= maxSat) {
					pq.push(x);

					pesimista = cotaPesimista(x.i, x.acum,n, m, minMatriz,x.mesas.size());
					if (pesimista > maxSat)maxSat = pesimista;
				}
			}
		}
	}

	std::cout << "Satisfaccion maxima: " << maxSat << '\n';
	for (int i = 0; i < n; ++i) {
		std::cout << "El comensal " << i << " va en la mesa " << sMejor[i] << '\n';
	}

}
/*Coste en tiempo en el orden del tamaño del espacio de soluciones por el coste de calculo de las cotas, O(m^n * n)
Coste en espacio adicional en O(m*n^2) (hay m*n nodos simultaneos como maximo y cada uno tiene un vector de tamaño n)
*/

bool resuelveCaso() {
	int n, m;
	std::cin >> n >> m;
	if (!std::cin)return false;

	std::vector<std::vector<int>> a(n, std::vector<int>(n));
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) {
			std::cin >> a[i][j];
		}
	}

	resolver(m, a);

	return true;
}

int main() {
	while (resuelveCaso());
}