#ifndef TABLE_H
#define TABLE_H

#include <string>

#pragma pack (push, 1)
struct Item {
    int key1;
    int key2;
    int len;
    std::string str;
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
    int load_from_file();
    void add(int k1, int k2, const std::string& s, int flag);
    void del(int k, int number);
    void find(int k, int number);
    static int hash(int k);
    void show() const;
    void upload_to_file();
};

#endif // TABLE.H
