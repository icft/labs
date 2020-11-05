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

const int MAX_SIZE = 2;

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
    std::istream& overriding_states(std::istream&);
    std::ostream& print(std::ostream&) const;
    void set_new_state(int, char);
    char get_state(int) const;
    void add_clem_connection(int, int);
    void reduce_clem_connection(int, int);
    void add_clem(struct clem);
};

#endif


