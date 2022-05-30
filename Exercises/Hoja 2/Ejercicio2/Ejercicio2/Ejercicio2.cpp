//Marcos Herrero

#include <iostream>
#include <vector>
#include <queue>
#include <cmath>
#include <algorithm>

struct moneda {
	int valor;
	int numero;

	bool operator<(moneda const & that)const {
		return valor > that.valor;// ordenamos de mayor a menor valor para facilitar el calculo de la cota optimista
	}
};

/*
Consideraremos las monedas ordenadas de mayor a menor valor

Restricciones explicitas:
La solucion es una tupla (s[0],...,s[n-1]) donde para todo 0<= i<= n-1, s[i] es un numero natural entre 0 y monedas[i].numero.
s[i] representa el numero de monedas pagadas del tipo i

Restricciones implicitas:
Toda tupla solucion valida ha de pagar la cantidad C de forma exacta, es decir, ha de verificar:
(sum: 0<= i <= n-1: s[i]*monedas[i].valor) = C

Objetivo: minimizar el numero de monedas utilizadas

*/

struct Nodo {
	std::vector<int> s; 
	int i; // se ha completado hasta s[i-1] . Es solucion completa si i = n, pero solo valida si porPagar = 0
	int numMonedas;// numero de monedas utilizadas en la solucion parcial
	double porPagar; //dinero que queda por pagar, es decir, C menos el dinero aportado por las monedas en la solucion parcial ( invariante: 0<= porPagar <=C)
	int optimista; // cota inferior del menor numero de monedas de una solucion completa valida

	bool operator<(Nodo const & that)const { // tiene mas prioridad el que tiene una cota optimista menor
		return optimista > that.optimista;
	}
};

/*
Cota optimista:
Nuestra cota optimista se obtiene al pagar la cantidad restante (porPagar) utilizando  monedas del mayor valor posible aun no considerado ,aunque paguemos mas de C, 
y sin respetar las restricciones de cardinalidad del tipo de moneda en cuestion 
( si respetaramos el numero maximo de cada tipo de monedas no habria garantia de que pudieramos pagar C o mas)
Coste de calculo: O(1) en tiempo (para facilitar el calculo de esta cota consideraremos las monedas ordenadas de mayor a menor valor)

No se puede utilizar el esquema optimista-pesimista, pues, sin mas informacion sobre el sistema monetario, no podemos garantizar que cualquier solucion parcial
sea completable a una solucion. De hecho, podria no existir ninguna solucion para el problema

Comentarios adicionales:
Si porPagar= 0, la solucion parcial se puede completar a exactamente una solucion completa, poniendo 0 monedas de los tipos restantes. Completamos directamente en esa
iteracion en lugar de volver a meter el nodo en la cola
*/

int cotaOptimista(int i, int numMonedas, double porPagar, std::vector<moneda> const & monedas) {//Coste O(1)
	return numMonedas + ceil(porPagar/monedas[i].valor);
}

void resolver(int C, std::vector<moneda>  & monedas) {
	int n = monedas.size();

	std::sort(monedas.begin(), monedas.end()); //ordenamos las monedas de mayor a menor valor. Intuitivamente parece que conviene probar antes con monedas de mayor valor
	
	std::priority_queue<Nodo> pq;
	std::vector<int> sMejor(n);
	int minMonedas = INT_MAX; // seria infinito
	int pesimista;

	Nodo x = { std::vector<int>(n), 0, 0, C, cotaOptimista(0,0,C,monedas) };
	pq.push(x);

	while (!pq.empty() && pq.top().optimista < minMonedas) {
		x = pq.top();
		pq.pop();

		++x.i;

		//los hijos del nodo x se diferencian en la cantidad que se paga de la moneda i, entre 0 y monedas[i].numero

		for (int k = monedas[x.i-1].numero; k >= 0; --k) {
			if (k*monedas[x.i - 1].valor <= x.porPagar) {
				x.s[x.i - 1] = k;
				x.numMonedas += k;
				x.porPagar -= k*monedas[x.i - 1].valor;

				if (x.porPagar == 0) { // en este caso solo hay esta completacion posible
					while (x.i < n) {
						x.s[x.i] = 0;
						++x.i;
					}
				}

				if (x.i == n) {
					if (x.porPagar == 0 && x.numMonedas < minMonedas) {
						sMejor = x.s;
						minMonedas = x.numMonedas;
					}
				}

				else {
					x.optimista = cotaOptimista(x.i, x.numMonedas, x.porPagar, monedas);

					if (x.optimista < minMonedas) pq.push(x);
				}

				x.numMonedas -= k;
				x.porPagar += k * monedas[x.i - 1].valor;
			}
		}
	}

	if (minMonedas == INT_MAX) std::cout << "No hay solucion\n";
	else {
		std::cout << "Minimo monedas: " << minMonedas << '\n';
		for (int i = 0; i < n; ++i) std::cout << "Monedas de valor " << monedas[i].valor << " : " << sMejor[i] << '\n';
		std::cout << '\n';
	}
}

/* El coste en tiempo en el caso peor esta en el orden del tamaño del espacio de soluciones que es O(prod: 0<= i<= n: (monedas[i].num + 1))
   ( en cada nodo todas las operaciones tienen coste cosntante)
   El coste en espacio adicional esta en O(n* sum: 0<= i <= n: (monedas[i].num)) (peso de cada nodo por el numero de nodos simultaneament en la cola)
*/

bool resuelveCaso() {
	int n, C;
	std::cin >> n >> C;
	if (!std::cin)return false;

	std::vector<moneda> monedas(n);
	for (int i = 0; i < n; ++i)std::cin >> monedas[i].valor;
	for (int i = 0; i < n; ++i)std::cin >> monedas[i].numero;

	resolver(C, monedas);
	
	return true;
}

int main() {
	while (resuelveCaso());
}