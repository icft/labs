#ifndef SCHEME_H
#define SCHEME_H

#include <iostream>
#include <stdexcept>
#include <list>

struct clem {
    Type type;
    int count;
    char signal;
};

enum Type {
    input = 0,
    output = 1,
};

class Scheme
{
private:
    int amount;
    struct clem* arr;
public:
    Scheme() {
        amount = 0;
        arr = nullptr;
    }
    ~Scheme() {
        delete[] arr;
    }
    Scheme(struct clem&);
    Scheme(int, int);
    Scheme(const struct clem*, int);
    Scheme(const Scheme&); //копирующий конструктор
    Scheme(Scheme&&); //перемещающий конструктор
    Scheme& operator= (const Scheme&); //оператор присваивания копирования
    Scheme& operator= (Scheme&&); //оператор присваивания перемещения
    //int find(int) const;
    friend std::istream& operator>> (std::istream&, Scheme&);
    friend std::ostream& operator<< (std::ostream&, const Scheme&);
    void operator() (int, struct clem&);
    struct clem operator[] (int) const;
    void add_clem_connection(int, int);
    void reduce_clem_connection(int, int);
    Scheme& operator+= (const Scheme&);
    friend const Scheme operator+ (const Scheme&, const Scheme&);
};

#endif
