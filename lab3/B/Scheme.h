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

const int MAX_SIZE = 5;

class Scheme
{
private:
    int amount;
    struct clem arr[MAX_SIZE];
public:
    Scheme() {
        amount = 0;
    }
    Scheme(int, int);
    Scheme(const struct clem*, int);
    int find(int) const;
    friend std::istream& operator>> (std::istream&, Scheme&);
    friend std::ostream& operator<< (std::ostream&, const Scheme&);
    void operator() (int, int);
    char operator[] (int) const;
    void add_clem_connection(int, int);
    void reduce_clem_connection(int, int);
    Scheme& operator+= (const Scheme&);
    const Scheme operator+ (const Scheme&);
};

#endif
