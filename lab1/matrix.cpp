#include "matrix.h"
#include <iostream>

double sum;

Matrix::Matrix(int a, int b) {
    m = a;
    n = b;
    arr = new std::list<Item>[a];
}

Matrix::~Matrix() {
    for (int i = 0; i < m; i++) {
        arr[i].clear();
    }
    delete [] arr;
}

int Matrix::add_matrix(int i, int j, double k) {
    Item f{};
    f.ind = j;
    f.key = k;
    if (arr[i].empty()) {
        arr[i].push_back(f);
        return 1;
    }
    auto it = arr[i].begin();
    while (j > it->ind)  {
        std::advance(it, 1);
        if (it == arr[i].end()) {
            arr[i].push_back(f);
            return 1;
        }
    }
    if (j == it->ind)
        return -1;
    arr[i].insert(it, f);
    return 1;
}

void Matrix::create_vec(std::vector<double>& vec) {
    for (int i = 0; i < m; i++) {
        int p;
        sum = 0;
        p = (i == 0) ? m-1 : i-1;
        std::cout << p << "\n";
        auto s = arr[p].begin();
        for (auto it = arr[i].begin(); it != arr[i].end(); std::advance(it, 1)) {
            s = find(p, it->ind, it->key, s);
        }
        vec[i] = sum;
    }
}

std::list<Item>::iterator Matrix::find(int i, int a, double k, std::list<Item>::iterator r) {
    int q = 0;
    auto it = r;
    for (; it != arr[i].end(); std::advance(it, 1)) {
        if (it->ind == a && it->key < k) {
            q = 1, sum += k;
            break;
        }
    }
    if (q == 0) {
        return r;
    } else {
        return it;
    }
}

void Matrix::print_matrix() const {
    std::cout << "Print matrix:\n";
    for (int i = 0; i < m; i++) {
        int c = 0;
        if (arr[i].empty()) {
            for (int q = 0; q < n; q++) {
                std::cout << 0 << " ";
            }
        } else {
            for (auto it = arr[i].begin(); it != arr[i].end(); it++) {
                while (c != it->ind) {
                    c++;
                    std::cout << 0 << " ";
                }
                std::cout << it->key << " ";
                c++;
            }
            while (c != n) {
                std::cout << 0 << " ";
                c++;
            }
        }
        std::cout << "\n";
    }
}

void print_vec(std::vector<double>& vec, int m) {
    std::cout << "Print vector:\n";
    for (int i = 0; i < m; i++)
        std::cout << vec[i] << " ";
    std::cout << "\n";
}
