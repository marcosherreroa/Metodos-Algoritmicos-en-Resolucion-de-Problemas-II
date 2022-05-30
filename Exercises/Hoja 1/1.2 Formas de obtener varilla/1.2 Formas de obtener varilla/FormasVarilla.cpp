//Marcos Herrero

#include <iostream>
#include <vector>

//Funcion recursiva: coste 2^n

int formasVarilla(std::vector<int> const& l, int fin, int L) {
    if (L < 0) return 0;
    else if (fin == 0) {
        if (L == 0)return 1;
        else return 0;
    }
    else {
        int s1 = formasVarilla(l, fin - 1, L - l[fin - 1]);
        int s2 = formasVarilla(l, fin - 1, L);

        return s1 + s2;
    }
}

//Algoritmo iterativo: coste n*L en tiempo y L en espacio adicional

bool resuelveCaso() {
    int n, L;
    std::cin >> n >> L;
    if (n == 0)return false;

    std::vector<int> l(n), c(n);
    for (int i = 0; i < n; ++i)std::cin >> l[i];
    for (int i = 0; i < n; ++i)std::cin >> c[i];

    std::vector<int> filaTabla(L + 1);

    filaTabla[0] = 1;
    for (int j = 1; j <= L; ++j)filaTabla[j] = 0;

    for (int i = 1; i <= n; ++i) {
        for (int j = L; j >= 0; --j) {
            int s1 = 0;
            if (j - l[i - 1] >= 0) s1 = filaTabla[j - l[i - 1]];
            int s2 = filaTabla[j];

            filaTabla[j] = s1 + s2;
        }
    }

    std::cout << filaTabla[L];

    return true;
}

//Coste

int main() {
    while (resuelveCaso());
}