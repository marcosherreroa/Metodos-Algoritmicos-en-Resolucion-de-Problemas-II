//Marcos Herrero

#include <iostream>
#include <vector>

using namespace std;

vector<int> factorizar(int n) {
	vector<int> factores;
	int i = 2;
	
	while (i <= sqrt(n)) {
		while (n % i == 0) {
			factores.push_back(i);
			n /= i;
		}
		++i;
	}

	if (n != 1)factores.push_back(n);
	return factores;
}

int main() {
	int n;
	while (std::cin >> n) {
		vector<int> v = factorizar(n);

		for (int i = 0; i < v.size(); ++i) {
			std::cout << v[i]<<' ';
		}
		std::cout << '\n';
	}
}
