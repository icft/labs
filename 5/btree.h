#ifndef BTREE_H
#define BTREE_H

#include <iostream>
#include <cstdlib>
#include <string>

typedef struct info {
    int key;
    std::string s;
} info;

typedef struct tree{
    int n, leaf;
    info * keys;
    struct tree ** children;
} btree;

btree* create_node();
btree* delete_tree(btree* );
void print( btree*, int );
info* search_key( btree*, int);
void search(btree*, int );
btree* insert_key( btree*, const info& );
btree* insert( btree*, const info& );
btree* remove_key( btree*, int );
btree* remove( btree*, int );
int sled_el(btree*, int, int);
void sled(btree*, int);
void inorder(btree*, int);
#endif //BTREE_H
