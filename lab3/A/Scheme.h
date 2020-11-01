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
    struct clem* arr[MAX_SIZE];
public:
    Scheme() {
        amount = 0;
        for (int i = 0; i < MAX_SIZE; i++) {
            arr[i] = nullptr;
        }
    }
    ~Scheme() {
        for (int i = 0; i < MAX_SIZE; i++) {
            delete arr[i];
        }
    }
    int find(int num);
    void init_count(int in, int out);
    void init_arr(const std::list<struct clem*>);
    void overriding_states();
    void print();
    void set_new_state(int ind);
    char get_state(int ind);
    void add_clem_connection(int ind);
    void reduce_clem_connection(int ind);
    void add_clem(struct clem*);
};

#endif
