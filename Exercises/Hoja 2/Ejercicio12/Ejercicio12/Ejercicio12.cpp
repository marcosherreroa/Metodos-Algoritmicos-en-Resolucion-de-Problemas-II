//Marcos Herrero

#include <iostream>
#include <vector>
#include <queue>
#include <unordered_set>

/*
Renumero las cañas de 0 a n-1
Numeramos los pilares de 0 a 3

Restricciones explicitas:
La solucion es una tupla (s[0],...s[n-1]) donde s[i] pertenece a {0,1,2,3} para todo 0<=i<=n-1 
s[i] representa el pilar al que se ha asignado la caña i ( siempre se utlizan todos los pilares)

Restricciones explicitas:
No hay, todas las tuplas anteriores son soluciones validas del porblema

Objetivo: maximizar la altura del pilar mas bajo. Es decir si altura[j] = (count: 0<= i <=n-1: s[i] = j) se quiere encontrar la solucion
cuyo valor  minAltura = min: 0<= j <= 3: altura[j] sea maximo

*/

struct Nodo {
	std::vector<int> s;//solucion parcial
	int i; // se ha completado hasta i-1. Es solucion completa si y solo si i =n
	std::vector<double> alturas; //alturas de los pilares en la solucion parcial
	double optimista; //cota superior de la maxima minima altura en una solucion alcanzable a partir de la parcial

	bool operator<(Nodo const & that)const {// mas prioridad el de cota optimista mayor
		return optimista < that.optimista;
	}
};

/*
Cota optimista:
Se basa en suponer que podemos fraccionar las cañas no utilizadas y que los pilares tienen todos aproximadadamente la altrua del mas alto (mejorarla, como Carlos)
Se obtiene por tanto sumando a la altura del pilar mas alto un cuarto de la suma de las alturas de los pilares restantes
Coste de caclulo: O(1) en cada llamada , mas O(n) de precomputo inicial de las sumas parciales. O(n) en espacio adicional

Cota pesimista:( se puede aplicar porque toda solucion parcial es completable a una solucion valida, al no haber restricciones implicitas)
Se basa en completar la solucion parcial asignando la caña siguiente al pilar mas bajo en cada iteracion.
Coste de calculo: O(n) en cada llamada

Poda adicional:
Los pilares se distinguen exclusivamente por su altura. Para evitar generar subarboles de soluciones equivalentes, cuando haya dos o mas pilares con la misma altura
solo probaremos a colocar la siguiente caña en uno de ellos ( colocarla en otro, daria una un subarbol de soluciones identicas salvo permutacion)
*/

double cotaOptimista(int i, std::vector<double> const & alturas, std::vector<double> const & sumas) {//coste O(1)
	double maxAltura = alturas[0];
	for (int k = 1; k < 4; ++k) {
		if (alturas[k] > maxAltura)maxAltura = alturas[k];
	}

	return maxAltura + sumas[i] / 4;
}

double cotaPesimista(int i, std::vector<double> const & alturas, std::vector<int> const & l) {//coste O(n)
	int n = l.size();
	std::priority_queue<double, std::vector<double>, std::greater<double>> altsaux;

	for (int k = 0; k < 4; ++k)altsaux.push(alturas[k]);

	double aux;
	for (int k = i; k < l.size(); ++k) {
		aux = altsaux.top();
		altsaux.pop();
		altsaux.push(aux + l[k]);
	}

	return altsaux.top();
}

void resolver(std::vector<int> const & l) {
	int n = l.size();

	std::vector<double> sumas(n);
	int suma = 0;
	for (int i = n - 1; i >= 0; --i) {
		suma += l[i];
		sumas[i] = suma;
	}

	std::priority_queue<Nodo> pq;
	std::vector<double> alturas(4, 0);
	std::vector<int> sMejor(n);
	double maxMinAltura = cotaPesimista(0, alturas, l);
	double pesimista, minAltura;
	std::unordered_set<double> usadas;

	Nodo x = { std::vector<int>(n), 0, alturas, cotaOptimista(0,alturas,sumas) };
	pq.push(x);

	while (!pq.empty() && pq.top().optimista >= maxMinAltura) {
		x = pq.top();
		pq.pop();

		++x.i;
	   usadas.clear();
		//no asignamos una caña a dos pilares con la mism altura

		for (int k = 0; k < 4; ++k) {
			if (!usadas.count(x.alturas[k])) {
				usadas.insert(x.alturas[k]);

				x.s[x.i - 1] = k;
				x.alturas[k] += l[x.i - 1];

				if (x.i == n) {
					minAltura = x.alturas[0];
					for (int i = 1; i < 4; ++i) {
						if (x.alturas[i] < minAltura)minAltura = x.alturas[i];
					}

					if (minAltura >= maxMinAltura) {
						sMejor = x.s;
						maxMinAltura = minAltura;
					}
				}

				else {
					x.optimista = cotaOptimista(x.i, x.alturas, sumas);

					if (x.optimista >= maxMinAltura) {
						pq.push(x);

						pesimista = cotaPesimista(x.i, x.alturas, l);
						if (pesimista > maxMinAltura)maxMinAltura = pesimista;
					}
				}


				x.alturas[k] -= l[x.i - 1];
			}
		}
	}

	std::cout << "La maxima minima altura es " << maxMinAltura << " y la asignacion optima de cañas a pilares:\n";
	for (int i = 0; i < n; ++i) {
		std::cout << "Caña " << i << " : Pilar " << sMejor[i]<<'\n';
	}
}
/*El coste en tiempo es el tamaño del espacio de soluciones multiplicado por el coste calcular las cotas en cada nodo, es decir, O(4^n*n)
El coste en espacio adicional esta en O(n^2)*
*/

bool resuelveCaso() {
	int n;
	std::cin >> n;
	if (n == 0)return false;

	std::vector<int> l(n);
	for (int i = 0; i < n; ++i)std::cin >> l[i];

	resolver(l);

	return true;
}

int main() {
	while (resuelveCaso());
}