#ifndef MATRIX_H
#define MATRIX_H

#include <list>
#include <vector>

struct Item {
    int ind;
    double key;
};

void print_vec(std::vector<double>&, int);

class Matrix {
private:
    int m{}; //количество строк
    int n{}; //количество столбцов
    std::list<Item> *arr;
public:
    Matrix(int, int);
    ~Matrix();
    int add_matrix(int, int , double);
    void create_vec(std::vector<double>&);
    void print_matrix() const;
    std::list<Item>::iterator find(int, int, double, std::list<Item>::iterator);
};
#endif //MATRIX_H