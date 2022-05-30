//Marcos Herrero

#include <iostream>
#include <vector>
#include <algorithm>


/*
1)

Definición recursiva:

  posible(i, j) vale true si es posible obtener la longitud j soldando varillas de la 0 a la i-1,  y false en caso contrario
  donde 0 <= i <= n y 0<= j <= L

  Casos base:

  posible(i,0) = 1 para todo 0<= i <= n;
  posible(0, j) = 0 para todo 1 <= j <= L;

  Casos recursivos:

  posible(i,j) = posible(i-1,j- l_{i-1}) || posible(i-1, j)  si i>=1 y j >= l_{i-1}
  posible(i,j) = posible(i-1, j)                             si i>= 1 y 1 <= j < l_{i-1}

*/

bool apartado1(std::vector<int> const & l, std::vector<int> const & c, int L) {
	int n = l.size();
	std::vector<bool> filaTabla(L + 1);

	filaTabla[0] = true;
	for (int j = 1; j <= L; ++j)filaTabla[j] = false;

	for (int i = 1; i <= n; ++i) {
		for (int j = L; j >= l[i - 1];  --j) {
			filaTabla[j] = filaTabla[j - l[i - 1]] || filaTabla[j];
		}
	}

	if (filaTabla[L]) {
		std::cout << "SI ";
		return true;
	}
	else {
		std::cout << "NO";
		return false;
	}
}

//  Coste O(n* L) en tiempo y O(L) en espacio adicional

/*
2)

Definicion recursiva:
   
   formas(i,j) es el numero de formas en que se puede obtener la longitud j soldando varillas desde 0 hasta i-1, sin que importe el orden
   donde 0 <= i <= n y 0<= j <= L

  Casos base: 

  formas(i,0) = 1 para todo 0<= i <= n
  formas(0,j) = 0 para todo 1<= j <= n

  Casos recursivos:

  formas(i,j) = formas(i-1,j- l_{i-1}) + formas(i-1, j)  si i>=1 y j >= l_{i-1}
  formas(i,j) = formas(i-1, j)                           si i>= 1 y 1 <= j < l_{i-1}

*/

void apartado2(std::vector<int> const & l, std::vector<int> const & c, int L) {
	int n = l.size();

	std::vector<int> filaTabla(L + 1);

	filaTabla[0] = 1;
	for (int j = 1; j <= L; ++j)filaTabla[j] = 0;

	for (int i = 1; i <= n; ++i) {
		for (int j = L; j >= l[i-1]; --j) {
			filaTabla[j] = filaTabla[j- l[i-1]] + filaTabla[j];
		}
	}

	std::cout << filaTabla[L]<<' ';
}

//Coste O(n*L) en tiempo y O(L) en espacio adicional

/*
3)

Definicion recursiva:

minVarillas(i,j) es el minimo numero de varillas necesarias para obtener la longitud j soldando varillas entre la 0 y la i-1
donde 0 <= i <= n y 0<= j <= L
Si no es posible, toma valor +infinito 

 Casos base:

 minVarillas(i, 0) = 0 para todo 0<= i <= n
 minVarillas(0, j) = +infinito para todo 1<= j <= n

 Casos recursivos:

 minVarillas(i,j) = min ( 1 + minVarillas(i-1, j-l_{i-1}) , minVarillas(i-1, j) )  si i>=1 y j >= l_{i-1}
 minVarillas(i,j) = minVarillas(i-1, j)                                            si i>= 1 y 1 <= j < l_{i-1}

*/

void apartado3(std::vector<int> const & l, std::vector<int> const & c, int L) {
	int n = l.size();
	int infinity = l.size() + 1;

	std::vector<int> filaTabla(L + 1);
	filaTabla[0] = 0;
	for (int j = 1; j <= L; ++j)filaTabla[j] = infinity;

	for (int i = 1; i <= n; ++i) {
		for (int j = L; j >= l[i-1]; --j) {
			filaTabla[j] = std::min(1 + filaTabla[j-l[i-1]], filaTabla[j]);
		}
	}

	std::cout << filaTabla[L]<<' ';
}
//Coste O(n*L) en tiempo y O(L) en espacio adicional

/*
4)

Definicion recursiva:

minPrecio(i,j) es el minimo precio de las  varillas necesarias para obtener la longitud j soldando varillas entre la 0 y la i-1
donde 0 <= i <= n y 0<= j <= L
Si no es posible, toma valor +infinito

 Casos base:

 minPrecio(i, 0) = 0 para todo 0<= i <= n
 minPrecio(0, j) = +infinito para todo 1<= j <= n

 Casos recursivos:

 minPrecio(i,j) = min ( c_{i-1} + minVarillas(i-1, j-l_{i-1}) , minVarillas(i-1, j) )  si i>=1 y j >= l_{i-1}
 minVarillas(i,j) = minVarillas(i-1, j)                                                si i>= 1 y 1 <= j < l_{i-1}

*/

void apartado4(std::vector<int> const & l, std::vector<int> const & c, int L) {
	int n = l.size();

	int infinity = 1;
	for (int i = 0; i < n; ++i)infinity += c[i]; // la suma de los costes más uno es mayor que cualquier coste que pueda salir

	std::vector<int> filaTabla(L + 1);

	filaTabla[0] = 0;
	for (int j = 1; j <= L; ++j)filaTabla[j] = infinity;

	for (int i = 1; i <= n; ++i) {
		for (int j = L; j >= l[i-1]; --j) {
			filaTabla[j]= std::min(c[i - 1] + filaTabla[j-l[i-1]], filaTabla[j]);
		}
	}

	std::cout << filaTabla[L]<<' ';
}
//Coste O(n*L) en tiempo y O(L) en espacio adicional


bool resuelveCaso() {
	int n, L;
	std::cin >> n >> L;
	if (!std::cin)return false;

	std::vector<int> l(n), c(n);
	for (int i = 0; i < n; ++i)std::cin >> l[i]>>c[i];
	
	if (apartado1(l, c, L)) {
		apartado2(l, c, L);
		apartado3(l, c, L);
		apartado4(l, c, L);
	}

	std::cout << '\n';
	return true;
}

int main() {
	while (resuelveCaso());
}