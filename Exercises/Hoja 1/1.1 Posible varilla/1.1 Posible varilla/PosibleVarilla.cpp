//Marcos Herrero

#include <iostream>
#include <vector>

bool posibleVarilla(std::vector<int> const & l, int fin, int L) {
    if (L < 0) return false;
    else if (fin == 0) {
        if (L == 0)return true;
        else return false;
    }
    else {
        bool s1 = posibleVarilla(l, fin - 1, L - l[fin-1]);
        bool s2 = posibleVarilla(l, fin - 1, L);

        return s1 || s2;
    }
}

bool resuelveCaso() {
    int n, L;
    std::cin >> n>> L;
    if (n == 0)return false;

    std::vector<int> l(n), c(n);
    for (int i = 0; i < n; ++i)std::cin >> l[i];
    for (int i = 0; i < n; ++i)std::cin >> c[i];

    /*std::vector<std::vector<bool>> tabla(n+1, std::vector<bool>(L));

    tabla[0][0] = true;
    for (int j = 1; j <= L; ++j)tabla[0][j] = false;

    for (int i = 1; i <= n; ++i) {
        for (int j = 0; j <= L; ++j) {
            bool s1 = false;
            if (j - l[i - 1] >= 0) s1 = tabla[i - 1][j - l[i - 1]];
            bool s2 = tabla[i - 1][j];

            tabla[i][j] = s1 || s2;
        }
    }*/
   
    
    std::vector<bool> filaTabla(L + 1);

    filaTabla[0] = true;
    for (int j = 1; j <= L; ++j)filaTabla[j] = false;

    for (int i = 1; i <= n; ++i) {
        for (int j = L; j >= 0; --j) {
            bool s1 = false;
            if (j - l[i - 1] >= 0) s1 = filaTabla[j - l[i - 1]];
            bool s2 = filaTabla[j];

            filaTabla[j] = s1 || s2;
        }
    }

    if (filaTabla[L])std::cout << "SI\n";
    else std::cout << "NO\n";

    return true;
}

int main() {
    while (resuelveCaso());
}