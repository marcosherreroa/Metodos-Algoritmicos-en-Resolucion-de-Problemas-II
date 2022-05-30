//Marcos Herrero

#include <iostream>
#include <vector>
#include <algorithm>

int minVarillas(std::vector<int> const& l, int fin, int L) {
    if (L < 0) return l.size()+1;//infinito
    else if (fin == 0) {
        if (L == 0)return 0;
        else return l.size()+1;
    }
    else {
        int s1 = minVarillas(l, fin - 1, L - l[fin - 1]);
        int s2 = minVarillas(l, fin - 1, L);

        return std::min(s1+1, s2);
    }
}

bool resuelveCaso() {
    int n, L;
    std::cin >> n >> L;
    if (n == 0)return false;

    std::vector<int> l(n), c(n);
    for (int i = 0; i < n; ++i)std::cin >> l[i];
    for (int i = 0; i < n; ++i)std::cin >> c[i];

    std::vector<int> filaTabla(L + 1);

    filaTabla[0] = 0;
    for (int j = 1; j <= L; ++j)filaTabla[j] = l.size()+1;

    for (int i = 1; i <= n; ++i) {
        for (int j = L; j >= 0; --j) {
            int s1 = l.size()+1;
            if (j - l[i - 1] >= 0) s1 = filaTabla[j - l[i - 1]];
            int s2 = filaTabla[j];

            filaTabla[j] = std::min(s1+1, s2);
        }
    }

    std::cout << filaTabla[L];

    return true;
}

int main() {
    while (resuelveCaso());
}