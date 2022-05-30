//Marcos Herrero

#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>

/*
Nota para mi: Como no se nos imponen restricciones sobre la altura de la pared, la dimension de un cuadro que decidamos colocar en vertical no afecta para nada
No obstante no podemos considerar que orientaciones distintas de un cuadro constituyan cuadros diferentes, ya que un mismo cuadro no se puede colocar
a la vez en vertical y en horizontal. Tampoco se puede descartar de entrada la medida que mas ocupa de cada cuadro, ya que no basta con que los cuadros ocupen menos
que L sino que se ha de conseguir que ocupen exactamente L 

Los cuadros se numeran de 0 a n-1

Restricciones explicitas:
Las soluciones del problema son tuplas (s[0],...,s[n-1]) donde s[i] pertenece a {-, A, B} para todo i entre 0 y n-1
s[i] = - representa que no se compra el cuadro i, s[i] = A significa que se compra y se coloca en vertical ( a[i] como base) y
s[i] = B significa que se compra  y se coloca en horizontal(b[i]) como base

Restricciones explicitas:
Para que una solucion sea valida, la suma de las longitudes de las bases de los cuadros escogidos ha de ser exactamente L, es decir:
(sum: 0<= i<= n-1 && s[i]= A: a[i])+(sum:0<= i <= n-1 && s[i] = B:b[i])= L

Objetivo: maximizar la suma de los prestigios de los cuadros comprados:
sum: 0<= i<= n-1 && s[i]!=- : p[i]
*/

struct Nodo {
	std::vector<char> s; // solucion parcial
	int i; // se ha completado hasta s[i-1]. La solucion es completa si y solo si i = n
	double acum; // prestigio acumulado por los cuadros ya comprados
	double libre; // espacio libre: L - espacio ocupado por loos cuadros ya comprados. Ha de ser 0<= libre <= L en toda solucion parcial
	double optimista; //cota superior del prestigio de la mejor solucion alcanzable a partir de la parcial

	bool operator<(Nodo const & that)const {//tiene mas prioridad el nodo con una cota optimista mayor
		return optimista < that.optimista;
	}
};

/*
Cota optimista:
Se obtiene al considerar que los cuadros se pueden fraccionar y aplicar el planteamiento voraz del problema de la mochila. 
Para ello se han de tener ordenados los cuadro  de mayor a menor por max(p[i]/a[i],p[i]/b[i]). Para calcular la cota optimista de una solucion parcial,
se consideran los cuadros restantes en este orden y se van comprando (añadiendo el prestigio que aportan al ya acumulado) y colocando en la posicion que menos ocupen
a lo largo de la pared, hasta que no quepan mas . Si cuando ya no caben mas queda espacio en la pared, el ultimo cuadro se añade fraccionado ( de nuevo, en la posicion que menos
ocuparia si estuviera completo). Coste calculo: O(n) mas O(nlogn) de ordenar al principio

No se puede aplicar el esquema optimista pesimista porque no hay garantia de que toda solucion parcial sea completable a una solucion que satisfaga las restricciones explicitas
Esto es porque, a diferencia de en el problema de la mochila, nos piden ocupar toda la pared de forma exacta, y en muchos casos esto no va a ser posible

Si libre = 0, la solucion parcial puede completarse de exactaemtne una forma: no comrpando ningun cuadro mas. Lo hacemos en el momento, sin volverlo a meter en la cola

*/

struct cuadro {
	int id;
	int p;
	int a;
	int b;

	bool operator<(cuadro const & that)const {// los ordenamos de mayor a menor por prestigio/espacio que ocupa
		return std::max(p/a,p/b) > std::max(that.p/that.a,that.p/that.b);
	}
};

int cotaOptimista(int i, double acum, int libre, std::vector<cuadro> const & cuadros) {//coste:O(n)
	int n = cuadros.size();

	while (i < n && std::min(cuadros[i].a, cuadros[i].b) <= libre) {
		acum += cuadros[i].p;
		libre -= std::min(cuadros[i].a, cuadros[i].b);
		++i;
	}

	if (i < n) {
		acum += cuadros[i].p*libre / std::min(cuadros[i].a, cuadros[i].b);
	}

	return acum;
}

void resolver(int L, std::vector<cuadro> const & cuadros) {
	int n = cuadros.size();

	std::priority_queue<Nodo> pq;
	std::vector<char> sMejor;
	int maxPrestigio = INT_MIN;//-inf

	Nodo x = { std::vector<char>(n), 0, 0, L, cotaOptimista(0,0,L,cuadros) };
	pq.push(x);
	
	while (!pq.empty() && pq.top().optimista > maxPrestigio) {
		x = pq.top();
		pq.pop();

		++x.i;

		if (cuadros[x.i-1].a <= x.libre) {
			x.s[x.i - 1] = 'A';
			x.acum += cuadros[x.i - 1].p;
			x.libre -= cuadros[x.i - 1].a;

			if (x.i == n) {
				if (x.libre == 0 && x.acum > maxPrestigio) {
					sMejor = x.s;
					maxPrestigio = x.acum;
				}
			}

			else {
				x.optimista = cotaOptimista(x.i, x.acum, x.libre, cuadros);
				if (x.optimista > maxPrestigio) pq.push(x);
			}

			x.acum -= cuadros[x.i - 1].p;
			x.libre += cuadros[x.i - 1].a;
		}

		if (cuadros[x.i - 1].b <= x.libre) {
			x.s[x.i - 1] = 'B';
			x.acum += cuadros[x.i - 1].p;
			x.libre -= cuadros[x.i - 1].b;

			if (x.i == n) {
				if (x.libre == 0 && x.acum > maxPrestigio) {
					sMejor = x.s;
					maxPrestigio = x.acum;
				}
			}

			else {
				x.optimista = cotaOptimista(x.i, x.acum, x.libre, cuadros);
				if (x.optimista > maxPrestigio) pq.push(x);
			}

			x.acum -= cuadros[x.i - 1].p;
			x.libre += cuadros[x.i - 1].b;
		}

		x.s[x.i - 1] = '-';
		if (x.i == n) {
			if (x.libre == 0 && x.acum > maxPrestigio) {
				sMejor = x.s;
				maxPrestigio = x.acum;
			}
		}

		else {
			x.optimista = cotaOptimista(x.i, x.acum, x.libre, cuadros);
			if (x.optimista > maxPrestigio) pq.push(x);
		}
	}

	if (maxPrestigio == INT_MIN)std::cout << "No hay solucion\n";
	else {
		std::cout << "Prestigio maximo: " << maxPrestigio<<'\n';
		for (int i = 0; i < n; ++i) {
			if (sMejor[i] == 'A')std::cout << "Se compra el cuadro " << cuadros[i].id << " de prestigio " << cuadros[i].p << " y se coloca en vertical\n";
			else if(sMejor[i] == 'B') std::cout<< "Se compra el cuadro " << cuadros[i].id << " de prestigio " << cuadros[i].p << " y se coloca en horizontal\n";
		}
	}

	std::cout << '\n';

}

/*El coste en el caso peor esta en el orden de la dimension del espacio de soluciones por el coste de calculo de las cotas, es decir, O(3^n * n)
El coste en espacio adicional esta en O(n^2) (hay 3*n nodos simultaneos en la cola y cada uno lleva un vector de n elementos)
*/

bool resuelveCaso() {
	int n, L;
	std::cin >> n >> L;
	if (!std::cin)return false;

	std::vector<cuadro> cuadros(n);
	for (int i = 0; i < n; ++i) {
		cuadros[i].id = i;
		std::cin >> cuadros[i].p >> cuadros[i].a >> cuadros[i].b;
	}

	std::sort(cuadros.begin(), cuadros.end());
	resolver(L, cuadros);

	return true;
}

int main() {
	while (resuelveCaso());
}