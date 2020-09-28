#include <iostream>
#include "matrix.h"

int main() {
    int a, b;
    std::cout << "Enter the dimensions of the matrix: ";
    std::cin >> a >> b;
    Matrix m(a, b);
    std::cout << "Input format: first index, second index, key. Indexes start at 1. To complete input, enter indexes in negative\n";
    int i = 3, j = 3;
    double k = 0;
    while (true) {
        std::cin >> i >> j >> k;
        if (i > a || j > b) {
            std::cout << "Indexes are greater than the allowed value\n";
            continue;
        }
        if (i <= 0 || j <= 0) {
            break;
        } else {
            int r = m.add_matrix(i-1, j-1, k);
            if (r == -1)
                std::cout << "Key with such indexes already exists\n";
        }
    }
    std::vector<double> vec(a, 0);
    m.create_vec(vec);
    m.print_matrix();
    print_vec(vec, a);
    vec.clear();
}