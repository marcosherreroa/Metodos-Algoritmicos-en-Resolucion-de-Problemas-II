//Marcos Herrero

#include <iostream>
#include <vector>
#include <algorithm>



int minPrecio(std::vector<int> const& l, std::vector<int> const & c, const int infinity,  int fin, int L) {//[ini, fin)
	if (L < 0) return infinity;//infinito
	else if (fin == 0) {
		if (L == 0)return 0;
		else return infinity;
	}
	else {
		int s1 = minPrecio(l, c, infinity, fin - 1, L - l[fin - 1]);
		int s2 = minPrecio(l, c, infinity, fin - 1, L);

		return std::min(s1 + c[fin-1], s2);
	}
}

bool resuelveCaso() {
	int n, L;
	std::cin >> n >> L;
	if (n == 0)return false;

	std::vector<int> l(n), c(n);
	for (int i = 0; i < n; ++i)std::cin >> l[i];
	for (int i = 0; i < n; ++i)std::cin >> c[i];

	int infinity = 1;
	for (int i = 0; i < n; ++i)infinity += c[i];

	std::vector<int> filaTabla(L + 1);

	filaTabla[0] = 0;
	for (int j = 1; j <= L; ++j)filaTabla[j] = infinity;

	for (int i = 1; i <= n; ++i) {
		for (int j = L; j >= 0; --j) {
			int s1 = infinity;
			if (j - l[i - 1] >= 0) s1 = filaTabla[j - l[i - 1]];
			int s2 = filaTabla[j];

			filaTabla[j] = std::min(s1 + c[i-1], s2);
		}
	}

	std::cout << filaTabla[L];

	return true;
}

int main() {
	while (resuelveCaso());
}