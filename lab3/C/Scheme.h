#ifndef SCHEME_H
#define SCHEME_H

#include <iostream>
#include <stdexcept>
#include <list>

struct clem {
    int number;
    int type;
    int count;
    char signal;
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
    Scheme(int, int);
    Scheme(const struct clem*, int);
    Scheme(const Scheme&);
    Scheme& operator= (const Scheme& scheme);
    int find(int);
    friend std::istream& operator>> (std::istream&, Scheme&);
    friend std::ostream& operator<< (std::ostream&, const Scheme&);
    void operator() (int, int);
    char operator[] (int);
    void add_clem_connection(int, int);
    void reduce_clem_connection(int, int);
    void operator+= (struct clem);
};

#endif

