//Marcos Herrero

#include <iostream>
#include <vector>
#include <queue>
#include <unordered_set>

/*
Los trabajos se numeran de 0 a n-1 y los procesadores de 0 a m-1

Restricciones explicitas:
Las soluciones del problema son tuplas de la forma (s[0],...,s[n-1]) donde s[i] pertenece a {0,...,m-1} para todo i
s[i]= j representa que el trabajo i se realiza en el procesador k. 
Con esta representacion , todos los trabajos se realizan en alguno de los m procesadores, como nos imponen

Restricciones explicitas:
No hay: no es nos impone ninguna restriccion aparte de que se realicen todos los trabajos, y eso ya esta contemplado en las restricciones
explicitas.

Objetivo: minimizar el tiempo empleado en realizar todos los trabajos, es decir, la cantidad
  max: 0<= i <= m-1: (sum:0<= j <= n-1 && s[j] = i : t[i])
(dicho de otra manera, partiendo de que se han de realizar todos los trabajos se pretnede minimizar el tiempo que tarda el procesador 
que mas tarde e realizar su labor)
*/

struct Nodo {
	std::vector<int> s;//solucion parcial
	int i;// se ha completado hasta i-1. Es solucion completa si y solo si i =n
	std::vector<double> acum; // tiempos acumulados en cada procesador por los trabajos ya asignados
	int indMax; //  indice entre 0 y m-1 que señala el procesador con mayor tiempo acumulado
	double optimista; // cota inferior del minimo tiempo que tardara el procesador que mas tarde en una solucion completable a a partir de esta

	bool operator<(Nodo const & that)const {//mas prioridad el que tenga cota optimista menor
		return optimista > that.optimista;
	}
};

struct proces {// tipo auxiliar para la pq de procesadores
	int id;
	double acum;

	bool operator<(proces const & that)const {//mas prioridad el que tenga menos acum
		return acum > that.acum;
	}
};

/*
Cota optimista:
Se basa en suponer que podremos repartir las tareas restantes en los procesadores sin que aumente el tiempo que tarda el procesador que mas
tiempo acumulado tiene y sin que ningun otro procesador acumule mas tiempo que el. En este contexto el maximo tiempo sera el que tenemos en la solucion parcial
  optimista = acum[indMax]
Coste de calculo: O(1). No hay precondicionamiento ni coste en espacio adicional

Cota pesimista:
Opcion 1: Completamos la solucion parcial asignando ,iterativamente ,los trabajos cada vez al procesador que menos tiempo acumulado tenga
Para ello utilizamos una cola de prioridad de procesadores que almacene indice-tiempoAcumulado y de mayor prioridad la que tenga menos
El coste del calculo de esta cota estaria en O(n logm). Parece un coste de calculo demasiado elevado para una cota pesimista, razon por la que
nos planteamos una alternativa mas simple
Coste de calculo en cada iteracion: O(nlogm)
Coste de precomputo: No hay
Coste en espacio adicional: O(m)

Opcion 2: Completamos la solucion parcial asignando todas las tareas al procesador menos ocupado en el momento ( el que tiene un tiempo aucmulado menor)
Por desgracia mantener actualizado que procesador tiene  menor tiempo acumulado no se puede hacer en tiempo constante. Lo mejor que se puede
hacer es llevar una cola de prioridad que se actualice cada vez que se añaden trabajos al porcesador con minimo tiempo(contrastadnola con el vector acum)
El coste de llamar a la funcion cotaPesimista seria constante, pero requiere en cada iteracion un trabajo en O(logm) (insertar/borrar elemento de la cola)
Ademas requiere llevar una cola de prioridad en cada nodo, ocupando mas espacio del convenido
Coste de calculo en cada iteracion: O(logm)
Coste de precomputo: O(m)
Coste en espacio adicional: O(m* numero de nodos en la cola en un momento dado(aprox m*n en el caso peor?))= O(m^2*n) ??

Valorando las alternativas , elegimos la Opcion1

Para evitar general soluciones equivalentes no añadimos un trabajo a dos procesadores que tengan exactamente el mismo tiempo acumulado: solo generamos
una de las ramas ya que la otra estaria formada exclusivamente por soluciones equivalentes (permutando procesadores)

*/

double cotaOptimista(std::vector<double> acum, int indMax) {//Coste O(1)
	return acum[indMax];
}

double cotaPesimista(int i, std::vector<double> const & acum, int indMax, std::vector<double> const & t) {//Coste(n logm)
	int n = t.size(), m = acum.size();
	double maxTiempo = acum[indMax];

	std::priority_queue<proces> pq;
	for (int k = 0; k < m; ++k) {
		pq.push({ k, acum[k] });
	}

	proces auxProcesador;
	for (int k = i; k < n; ++k) {
		auxProcesador = pq.top();
		pq.pop();

		auxProcesador.acum += t[k];
		if (auxProcesador.acum > maxTiempo) {
			indMax = auxProcesador.id;
			maxTiempo = auxProcesador.acum;
		}

		pq.push(auxProcesador);
	}
	
	return maxTiempo;
}

void resolver(int m, std::vector<double> const & t) {
	int n = t.size();

	std::priority_queue<Nodo> pq;
	std::vector<int> sMejor;
	std::vector<double> acum(m, 0.0);
	double minMaxTiempo = cotaPesimista(0, acum, 0, t);
	std::unordered_set<double> usadas;
	int auxIndMax;

	Nodo x = { std::vector<int>(n), 0,acum, 0, cotaOptimista(acum,0) };
	pq.push(x);
	while (!pq.empty() && pq.top().optimista <= minMaxTiempo) {
		x = pq.top();
		pq.pop();

		++x.i;
		usadas.clear();
		//no asignamos un trabajo a dos procesadores con la misma ocupacion, para no generar soluciones equivalentes

		for (int k = 0; k < m; ++k) {
			if (usadas.count(x.acum[k]) == 0) {
				usadas.insert(x.acum[k]);
				x.s[x.i - 1] = k;
				x.acum[k] += t[x.i-1];
				auxIndMax = x.indMax;
				if (x.acum[k] > x.acum[x.indMax])x.indMax = k;

				if (x.i == n) {
					if (x.acum[x.indMax] <= minMaxTiempo) {
						sMejor = x.s;
						minMaxTiempo = x.acum[x.indMax];
					}
				}
				else {
					x.optimista = cotaOptimista(x.acum, x.indMax);

					if (x.optimista <= minMaxTiempo) {
						pq.push(x);

						minMaxTiempo = std::min(minMaxTiempo , cotaPesimista(x.i, x.acum, x.indMax, t));
					}
				}

				x.acum[k] -= t[x.i - 1];
				x.indMax = auxIndMax;
			}
		}
	}

	std::cout << "El minimo tiempo que tardaran los procesadores en completar todos los trabajos es " << minMaxTiempo << "\nCon la siguiente asignacion:\n";
	for (int i = 0; i < n; ++i) {
		std::cout << "Trabajo " << i << ": Procesador " << sMejor[i] << '\n';
	}
}
/*El coste en tiempo en el caso peor esta en el orden del tamaño del espacio de soluciones mutliplicado por el coste de las cotas: O(m^n* nlogm).
No obstante, como no tratamos las soluciones equivalentes, este coste se puede dividir por el numero de permutaciones de los procesadores, 
es decir, esta en O((m^n* n * logm)/m!)
 El coste en espacio adicional en el caso peor esta en O(espacio adicional de cada nodo* numero de nodos simultaneos) = O(m* n*m)= O(m^2*n)
 */

bool resuelveCaso() {
	int n, m;
	std::cin >> n >> m;
	if (!std::cin)return false;

	std::vector<double> t(n);
	for (int i = 0; i < n; ++i)std::cin >> t[i];

	resolver(m, t);
	return true;
}

int main() {
	while (resuelveCaso());
}