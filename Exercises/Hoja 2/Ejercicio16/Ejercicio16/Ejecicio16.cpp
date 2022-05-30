//Marcos Herrero

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <queue>

struct asignatura {
	std::string nombre;
	int f;
	int d;
	double v;

	bool operator<(asignatura const & that)const {
		return f < that.f;
	}
};
/*
Numeramos las asignaturas de 0 a n-1. Las consideramos ordenadas de menor a mayor por los dias que faltan desde el dia 0 hasta el examen.

Dada una seleccion de asignaturas que a Juanita le daria tiempo a estudiar, puede haber varias ordenaciones posibles del mismo en las que le
daria tiempo a todas. Sin embargo, el orden de estudio de menor a mayor fecha de examen siempre funciona ( si es que la seleccion de asignaturas es valida)
Por tanto no hemos de preocuparnos por el orden de estudio, sino solo de que asignaturas estudia, suponiendo que las estudia en el orden optimo,
que es de menor a mayor fecha de examen.

Restricciones explicitas:
Las soluciones del problema son tuplas de de  n elementos (s[0],...,s[n-1]) donde s[i] pertenece a {0,1}
s[i] = 0 representa que Juanita no estudia la asignatura i y s[i] = 1 que si la estudia

Restricciones implicitas:
Para poder estudiar una asignatura, es necesario que la suma del tiempo invertido en estudiar esa y todas las anteriores(que se estudien) sea
menor o igual que los dias que faltan para el examen. Es decir las tuplas solucion (s[0],...,s[n-1]) validas han de verificar:
Para todo 0<=i<= n-1 tal que s[i]=1 , se da (sum:0<= k<= i-1: s[k]*d[k])+d[i] <= f[i]

(pese a estas restricciones toda solucion parcial es completable a una solucion, pues  siempre se puede elegir no estudiar ninguna de las 
asignaturas sucesivas, de manera que no se viola ninguna restrccion implicita)

Objetivo:
Maximizar la suma de la os valores de las asingaturas estudiadas , es decir, sum:0<= i<= n-1: s[i]*v[i]

*/

struct Nodo {
	std::vector<int> s;//solucion parcial
	int i; // se ha completado hasta i-1. Solucion completa si y solo si i = n
	double acum; // valor acumulado de las asignaturas estudiadas dentro de plazo
	int dia; //dia en que nos encontramos despues de haber realizado el estudio o no estudio de las asignaturas de 0 a i-1  elegidas
	double optimista; // cota superior del mayor valor que se puede obtener al completar la solucion parcial

	bool operator<(Nodo const & that)const {// mas prioridad la cota optimista mayor
		return optimista < that.optimista;
	}
};

/*
Cota optimista:
Suponer que da tiempo a estudiar todas las asignaturas restantes, de manera que
 optimista = acum + sum: i<= k<= n-1: v[k]
Llevando precalculadas las sumas parciales del vector de valores (sumas[i] = sum: i<= k<= n-1: v[i]) el coste calcular
la cota en cada nodo es constante

Cota pesimista:
Dos opciones :
  1 Suponer que no da tiempo a estudiar ninguna asignatura mas : pesimista = acum
  2 Completar la solucion, se recorren ordenadamente las asignaturas y para cada una se elige estudiarla siempre que se este a tiempo
Optamos por la cota 1 por tener un coste de calculo constante y no haber indicios de que la segunda cota vaya a ser mucho mayor

*/

double cotaOptimista(int i, double acum, std::vector<double> const & sumas) {//Coste O(1)
	return acum + sumas[i];
}

double cotaPesimista(int i, double acum) {//Coste O(1)
	return acum;
}

void resolver(std::vector<asignatura> const & A) {
	int n = A.size();

	std::vector<double> sumas(n);
	int suma = 0;
	for (int i = n - 1; i >= 0; --i) {//Coste O(n)
		suma += A[i].v;
		sumas[i] = suma;
	}

	std::priority_queue<Nodo> pq;
	std::vector<int> sMejor;
	int maxValor = cotaPesimista(0, 0);
	int pesimista;

	Nodo x = { std::vector<int>(n), 0, 0, 0, cotaOptimista(0,0,sumas) };
	pq.push(x);

	while (!pq.empty() && pq.top().optimista >= maxValor) {
		x = pq.top();
		pq.pop();

		++x.i;

		//Hijo 1: se estudia
		if (A[x.i - 1].f - A[x.i - 1].d >= x.dia) { // si da tiempo a estudiarlo
			x.s[x.i - 1] = 1;
			x.acum += A[x.i - 1].v;
			x.dia += A[x.i - 1].d;

			if (x.i == n) {
				if (x.acum >= maxValor) {
					sMejor = x.s;
					maxValor = x.acum;
				}
			}

			else {
				//la cota optimista es la misma que la del padre

				if (x.optimista >= maxValor) {
					pq.push(x);

					pesimista = cotaPesimista(x.i, x.acum);
					if (pesimista > maxValor) maxValor = pesimista;
				}
			}

			x.acum -= A[x.i - 1].v;
			x.dia -= A[x.i - 1].d;
		}

		//Hijo 2: no se estudia
			x.s[x.i - 1] = 0;
			if (x.i == n) {
				if (x.acum >= maxValor) {
					sMejor = x.s;
					maxValor = x.acum;
				}
			}

			else {
				x.optimista = cotaOptimista(x.i, x.acum, sumas);

				if (x.optimista >= maxValor) {
					pq.push(x);

					//la cota pesimista es la misma que la del padre
				}
			}
	}
}
/*
El coste del algoritmo en el caso peor esta en el orden del tamaño del espacio de soluciones por el coste de las operaciones realizadas en cada nodo
Como el calculo de las cotas tiene coste constante, en cada nodo solo se realizan operaciones de coste constante, asi que el coste esta en el orden
 del tamaño del espacio de soluciones, O(2^n) (las soluciones son tuplas de n elementos, cada uno de los cuales puede tomar 2 valores)

 El coste en espacio adicional esta en O(n^2) ( 2*n nodos simultaneaos y cada uno de ellos lleva un vector de n elementos)
 */

bool resuelveCaso() {
	int n;
	std::cin >> n;
	if (n == 0)return false;

	std::vector<asignatura> A(n);
	for (int i = 0; i < n; ++i) {
		std::cin >>A[i].nombre>> A[i].f >> A[i].d >> A[i].v;
	}

	resolver(A);

	return true;
}

int main() {
	while (resuelveCaso());
}-