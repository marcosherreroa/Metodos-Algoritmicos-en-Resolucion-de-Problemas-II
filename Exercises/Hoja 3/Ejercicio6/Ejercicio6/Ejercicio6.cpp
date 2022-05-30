//Marcos Herrero

#include <iostream>
#include <string>
#include <vector>

void preprocesado(std::string const & T1, std::string const & T2, std::vector<int> & pi1) {
	int n = T1.size()-1;

	pi1[1] = 0;
	int k = 0;

	for (int q = 2; q < n; ++q) {
		while (k > 0 && T1[k + 1] != T2[q])k = pi1[k];
		if (T1[k + 1] == T2[q])++k;
	}
}

bool circular(std::string const & T1, std::string const & T2) {

}

bool resuelveCaso() {
	int n;
	std::cin >> n;
	if (n == 0)return false;

	std::string T1, T2;
	std::cin >> T1 >> T2;

	if (circular(T1, T2))std::cout << "SI\n";
	else std::cout << "NO\n";

	return true;
}

int main() {
	while (resuelveCaso());
}