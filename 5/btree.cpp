#include "btree.h"
#include <malloc.h>

const int t = 2;
int pred;

btree* create_node() {
    btree* node = (btree*)malloc(sizeof(btree));
    node->n = 0;
    node->keys = (info*)malloc(sizeof(info)*(2*t-1));
    node->leaf = 1;
    node->children = (btree**)malloc(sizeof(btree)*2*t);
    for (int i = 0; i < 2*t; i++)
        node->children[i] = nullptr;
    return node;
}

btree* split_node(btree* x, int i, btree* y) {
    btree* z = create_node();
    z->n = t - 1;
    z->leaf = y->leaf;
    unsigned j;
    for(j = 0; j < t - 1; j++)
        z->keys[j] = y->keys[j + t];
    if(!y->leaf)
        for(j = 0; j < t; j++){
            z->children[j] = y->children[j+t];
            y->children[j + t] = nullptr;
        }
    y->n = t - 1;
    for(j = x->n; j >= i; j--)
        x->children[j + 1] = x->children[j];
    x->children[i] = z;
    for(j = x->n; j >= i; j--)
        x->keys[j] = x->keys[j - 1];
    x->keys[i- 1] = y->keys[t - 1];
    x->n++;
    return x;

}

btree* insert_key(btree* x, const info& in) {
    int i = x->n - 1;
    if(x->leaf){
        while((i >= 0) && (in.key < x->keys[i].key)){
            x->keys[i + 1] = x->keys[i];
            i--;
        }
        x->keys[i + 1] = in;
        x->n++;
        return x;
    }
    while((i >= 0) && (in.key < x->keys[i].key)) i--;
    i++;
    if(x->children[i]->n == (2 * t) - 1){
        x = split_node(x, i+1, x->children[i]);
        if(in.key > x->keys[i].key) i++;
    }
    x->children[i] = insert_key(x->children[i], in);
    return x;
}

info* search_key( btree* T, int key) {
    int i = 0;
    while(i < T->n && key > T->keys[i].key) i++;
    if(i < T->n && key == T->keys[i].key) {
        return &T->keys[i];
    }
    if(T->leaf) return nullptr;
    return search_key(T->children[i], key);
}

btree* remove_key(btree* node, int key) {
    int i;
    for (i = 0; i < node->n && key > node->keys[i].key; i++);
    if (i < node->n && key == node->keys[i].key) {
        if (node->leaf) {
            for (int j = i; j < node->n-1; j++)
                node->keys[j] = node->keys[j+1];
            node->n--;
            return node;
        }
        if(!node->leaf && node->children[i]->n >= t) {
            btree* node_ = node->children[i];
            while (!node_->leaf) {
                node_ = node_->children[0];
            }
            info tmp = node_->keys[0];
            node_ = remove_key(node_, tmp.key);
            node->keys[i] = tmp;
            return node;
        }
        if(!node->leaf && node->children[i + 1]->n >= t) {
            btree* node_ = node->children[i + 1];
            while(!node_->leaf)
                node_ = node_->children[0];
            info tmp  = node_->keys[0];
            node_ = remove_key(node->children[i + 1], tmp.key);
            node->keys[i] = tmp;
            return node;
        }
        if(!node->leaf && node->children[i + 1]->n == t - 1 && node->children[i]->n == t - 1) {
            btree* y = node->children[i];
            btree* z = node->children[i + 1];
            y->keys[y->n].key = key;
            int j;
            for(j = 0; j < t - 1; j++)
                y->keys[t + j] = z->keys[j];
            for(j = 0; j <= t; j++)
                y->children[t + j] = z->children[j];
            y->n = 2 * t - 1;
            for(j = i; j < node->n - 1; j++)
                node->keys[j] = node->keys[j + 1];
            for(j = i + 1; j <= node->n; j++)
                node->children[j] = node->children[j + 1];
            node->children[j] = nullptr;
            node->n--;
            node->children[i] = remove_key(node->children[i], key);
            return node;
        }
    }
    btree* y = node->children[i];
    btree* z = nullptr;
    if(y->n == t - 1) {
        if(i < node->n && node->children[i + 1]->n >= t) {
            z = node->children[i + 1];
            y->keys[t - 1] = node->keys[i];
            y->n++;
            node->keys[i] = z->keys[0];
            for(int j = 0; j < z->n - 1; j++)
                z->keys[j] = z->keys[j + 1];
            y->children[y->n] = z->children[0];
            for(int j = 0; j < z->n; j++)
                z->children[j] = z->children[j + 1];
            z->n--;
            node->children[i] = remove_key(node->children[i], key);
            return node;
        }
        if(i > 0 && !z && node->children[i - 1]->n >= t) {
            z = node->children[i - 1];
            unsigned j;
            for(j = y->n; j > 0; j--)
                y->keys[j] = y->keys[j - 1];
            for(j = y->n + 1; j > 0; j--)
                y->children[j] = y->children[j - 1];
            y->keys[0] = node->keys[i - 1];
            y->n++;
            node->keys[i - 1] = z->keys[z->n - 1];
            y->children[0] = z->children[z->n];
            z->n--;
            node->children[i] = remove_key(y, key);
            return node;
        }
        if(!z) {
            if(i < node->n && node->children[i + 1]->n == t - 1){
                z = node->children[i + 1];
                y->keys[t - 1] = node->keys[i];
                y->n++;
                unsigned j;
                for(j = 0; j < t - 1; j++){
                    y->keys[t + j] = z->keys[j];
                    y->n++;
                }
                if(!y->leaf)
                    for (j = 0; j < t; j++)
                        y->children[t + j] = z->children[j];
                for(j = i; j < node->n - 1; j++){
                    node->keys[j] = node->keys[j + 1];
                    node->children[j + 1] = node->children[j + 2];
                }
                node->n--;
                node = remove_key(node, key);
                return node;
            }
            if((i > 0) && (node->children[i - 1]->n == t - 1)){
                z = node->children[i - 1];
                if(i == node->n)
                    z->keys[t - 1] = node->keys[i - 1];
                else
                    z->keys[t - 1] = node->keys[i];
                z->n++;
                int j;
                for(j = 0; j < t - 1; j++){
                    z->keys[t + j] = y->keys[j];
                    z->n++;
                }
                if(!z->leaf)
                    for(j = 0; j < t; j++)
                        z->children[t + j] = y->children[j];
                node->n--;
                node->children[i - 1] = z;
                node = remove_key(node, key);
                return node;
            }
        }
    }
    node->children[i] = remove_key(node->children[i], key);
    return node;
}

btree* insert(btree* T, const info& in) {
    if(!T){
        T = create_node();
        T->keys[0] = in;
        T->n++;
        return T;
    }
    if(T->n == 0) {
       T =  insert_key(T, in);
       return T;
    }
    info* q;
    q = search_key(T, in.key);
    if (q) {
        std::cout << "Такой ключ существует\n";
        return T;
    }
    if(T->n == (2 * t) - 1){
        btree* aux = create_node();
        aux->leaf = 0;
        aux->children[0] = T;
        aux = split_node(aux, 1, T);
        aux = insert_key(aux, in);
        return aux;
    }
    T = insert_key(T, in);
    return T;
}

btree* remove(btree* T, int key) {
    info* q;
    if (!T) {
        std::cout << "Дерево пустое\n";
        return T;
    }
    q = search_key(T, key);
    if (!q) {
        std::cout << "Элемент с таким ключом не найден\n";
        return T;
    }
    return remove_key(T, key);
}

void search(btree* T, int key) {
    if (!T) {
        std::cout << "Дерево пустое\n";
        return;
    }
    info*  q;
    q = search_key(T, key);
    if (!q) {
        std::cout << "Элемента с таким ключом не существует\n";
        return;
    }
    std::cout << q->key << q->s;
}

btree* delete_tree(btree* T) {
    if (!T)
        return nullptr;
    if(!T->leaf) {
        for (int i = 0; i < T->n; i++)
            delete_tree(T->children[i]);
    }
    delete[] T->keys;
    delete[] T->children;
    delete T;
    return nullptr;
}

void inorder(btree* T, int l) {
    for (int i = 0;i <= T->n;i++) {
        if (T->children[i] != nullptr)
            inorder(T->children[i],l+1);
        if (i < T->n) {
            std::cout << T->keys[i].key << " " << T->keys[i].s << "\n";
        }
    }
}

void print(btree* T, int l) {
    for (int i = 0;i <= T->n;i++) {
        if (T->children[i] != nullptr)
            print(T->children[i],l+1);
        if (i < T->n) {
            for (int j = 0;j < l;j++)
                std::cout << "  ";
            std::cout << T->keys[i].key << "\n";
        }
    }
}

info* max_el(btree* T) {
    for (int i = T->n;i >= 0;i--) {
        if (T->children[i] != nullptr)
            return max_el(T->children[i]);
    }
    return &T->keys[T->n-1];
}
