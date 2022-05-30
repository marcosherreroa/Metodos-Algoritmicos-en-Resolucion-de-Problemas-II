//Marcos Herrero

#include <iostream>
#include <vector>
#include <algorithm>



class infinityType {
public:
	long long int num;
	bool isInf;

	infinityType() : num(), isInf(false) {};

	infinityType(long long int _num, bool _inf) : num(_num), isInf(_inf) {};

	infinityType(long long int _num) : num(_num), isInf(false) {};

	infinityType & operator=(long long int that) {
		num = that;
		isInf = false;
		return *this;
	}

	bool operator<(infinityType const & that)const {
		return that.isInf || !isInf && num < that.num;
	}

	bool operator<=(infinityType const & that)const {
		return that.isInf || !isInf && num <= that.num;
	}

	bool operator==(infinityType const & that)const {
		if (isInf != that.isInf)return false;
		else if (isInf && that.isInf)return true;
		else return num == that.num;
	}

	bool operator<(int that)const {
		return !isInf && num < that;
	}

	infinityType operator+(infinityType const & that) {
		if (isInf || that.isInf)return infinityType(0,true);
		else return { num + that.num, false };
	}

};

bool operator<(long long int a, infinityType const & b) {
	return b.isInf || a < b.num;
}

bool operator<=(long long int a, infinityType const & b) {
	return b.isInf || a <= b.num;
}

bool operator==(long long int a, infinityType const & b) {
	return !b.isInf &&  a == b.num;
} 

infinityType max(infinityType const & a, infinityType const & b) {
	if (a < b)return b;
	else return a;
}

infinityType min(infinityType const & a, infinityType const & b) {
	if (b < a)return b;
	else return a;
}

const infinityType inf = infinityType(0, true);

infinityType operator+(long long int a, infinityType const & b) {
	if (b.isInf)return inf;
	else return  { a + b.num, false };
}

struct sol {
	infinityType coste;
	infinityType potencia;
};

/*
Def:
minCoste(i, j) es el minimo coste de  iluminar con las bombillas 0,...,i-1 para obtener una potencia en [j,j+PMax-PMin]
minPotencia(i,j) es la minima potencia en [j,j+PMax-PMin] de iluminar con las bombillas 0,...,i-1 con coste minCodte(i,j)
0<= i <= N, 0<= j <= PMin

minCoste(i,0) =  0 para todo i 
minCoste(0, j) = inf para todo j>0
minPotencia(i,0) = 0 para todo i
minPotencia(0, j) = inf para todo j > 0


Si i>0, j-p[i-1] >= 0:  al usarla no superamos la potencia minima
    Si c[i-1] + minCoste(i, j-p[i-1]) < minCoste(i-1,j)  o c[i-1] + minCoste(i, j-p[i-1]) = minCoste(i-1,j) y p[i-1] + minCoste(i,j-p[i-1]) <= minPotencia(i-1, j):  
	      minCoste(i,j) = c[i-1] + minCoste(i,j-p[i-1])
		  minPotencia(i,j) = p[i-1] + minCoste(i,j-p[i-1])
   
   Si c[i-1] + minCoste(i, j-p[i-1]) > minCoste(i-1,j) o c[i-1] + minCoste(i, j-p[i-1]) = minCoste(i-1,j) y p[i-1] + minCoste(i,j-p[i-1]) > minPotencia(i-1, j) : 
          minCoste(i,j) = minCoste(i-1,j)
		  minPotencia(i,j) = minPotencia(i-1, j)

Si i>0, j-p[i-1] < 0  y j-p[i-1]+PMax-PMin >= 0: al usarla superamos la potencia minima pero no la maxima
   Si c[i-1]  < minCoste(i-1,j)  o c[i-1] = minCoste(i-1,j) y p[i-1] <= minPotencia(i-1, j):  
	      minCoste(i,j) = c[i-1]
		  minPotencia(i,j) = p[i-1]
	
	Si c[i-1]  > minCoste(i-1,j) o c[i-1] = minCoste(i-1,j) y p[i-1] > minPotencia(i-1, j) :
		  minCoste(i,j) = minCoste(i-1,j)
		  minPotencia(i,j) = minPotencia(i-1, j)

Si i>0, j-p[i-1]+PMax-PMin < 0 : al usarla nos pasamos de la potencia maxima
   minCoste(i,j) = minCoste(i-1,j)
   minPotencia(i,j) = minPotencia(i-1,j)

*/

void resolver(long long int PMax, long long int PMin, std::vector<long long int> const & p, std::vector<long long int> const & c) {
	long long int N = p.size();

	std::vector<infinityType> minCoste(PMin + 1);
	std::vector<infinityType> minPotencia(PMin + 1);

	minCoste[0] = 0;
	minPotencia[0] = 0;
	for (long long int j = 1; j <= PMin; ++j) {
		minCoste[j] = inf;
		minPotencia[j] = inf;
	}

	for (long long int i = 1; i <= N; ++i) {
		long long int j = std::max ((long long) 1 , p[i-1]- PMax + PMin); // para j menores , si usaramos la bombilla nos pasariamos de la potencia maxima

		while (j <= PMin && j < p[i - 1]) { // al usarla superamos la potencia minima pero no la maxima
			if (c[i - 1] < minCoste[j] || (c[i - 1] == minCoste[j] && p[i - 1] <= minPotencia[j])) {
				minCoste[j] = c[i - 1];
				minPotencia[j] = p[i - 1];
			}
			
			++j;
		}

		while (j <= PMin) {// al usarla no superamos la potencia minima
			if (c[i - 1] + minCoste[j - p[i - 1]] < minCoste[j] ||
				(c[i - 1] + minCoste[j - p[i - 1]] == minCoste[j] && p[i - 1] + minPotencia[j - p[i - 1]] <= minPotencia[j])) {
				minCoste[j] = c[i - 1] + minCoste[j - p[i - 1]];
				minPotencia[j] = p[i - 1] + minPotencia[j - p[i - 1]];
			}
			
			++j;
		}
	}

	if (!minCoste[PMin].isInf) std::cout << minCoste[PMin].num << ' ' << minPotencia[PMin].num<<'\n';
	else std::cout << "IMPOSIBLE\n";

}

bool resuelveCaso() {
	long long int N, PMax, PMin;
	std::cin >> N >> PMax >> PMin;
	if (!std::cin)return false;

	std::vector<long long int> p(N), c(N);
	for (long long int i = 0; i < N; ++i)std::cin >> p[i];
	for (long long int i = 0; i < N; ++i)std::cin >> c[i];

	resolver(PMax, PMin, p, c);


	return true;
}

int main() {
	while (resuelveCaso());
}