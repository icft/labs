#ifndef TABLE_H
#define TABLE_H

#include <string>

#pragma pack (push, 1)
struct Item {
    int del; // 1-удалена
    int key1;
    int key2;
    int len;
    int off;
    int ind1;
    int ind2;
};
#pragma pack (pop)

struct Space {
    Item *a;
    int busy;
    Space() {
        a = nullptr;
        busy = 0;
    }
};

const int M = 31;

class Table {
private:
    Space pk1[M];
    Space pk2[M];
public:
    ~Table();
    int load_from_file(const std::string& filename);
    void add_from_file(int d, int k1, int k2, int l, int offset);
    void add(int k1, int k2, const std::string& s, const std::string& filename);
    void del(int k, int number, const std::string& filename);
    void find(int k, int number, const std::string& filename);
    static int hash(int k);
    void show(const std::string& filename) const;
    void update(const std::string& filename);
};

#endif // TABLE.H
