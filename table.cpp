#include "table.h"
#include <fstream>
#include <string>
#include <iostream>
#include <vector>

Table::~Table() {
    for(auto & i : pk1)
    {
        delete i.a;
    }
}

static int h = 3;
int size = 0;
int offset = 0;

int Table::load_from_file(const std::string& filename) {
    std::ifstream f(filename, std::ios::in | std::ios::binary);
    if (!f.is_open()) {
        return -1;
    }
    else {
        while (true) {
            if (size == M) {
                size = 0;
                f.close();
                return 0;
            }
            int d = 0;
            int k1 = 0;
            int k2 = 0;
            int l = 0;
            int of = 0;
            f.read((char *)&d, sizeof(int));
            if (f.eof()) {
                break;
            }
            f.read((char *)&k1, sizeof(int));
            f.read((char *)&k2, sizeof(int));
            f.read((char *)&l, sizeof(int));
            f.read((char*)&of, sizeof(int));
            f.seekg(l, std::ios_base::cur);
            add_from_file(d, k1, k2, l, of);
        }
        f.close();
        return 1;
    }
}

void Table::add(int k1, int k2, const std::string& s, const std::string& filename) {
    int pos, i1, i2;
    int c = 0;
    pos = i1 = hash(k1);
    while (pk1[i1].busy > 0) {
        if (pk1[i1].a->key1 == k1 and pk1[i1].a->key2 == k2) {
            c = -1;
            break;
        }
        i1 = (i1 + h) % M;
        if (i1 == pos) {
            c = -2;
            break;
        }
    }
    if (c ==-1) {
        std::cout << "В таблице уже есть элемент с такими ключами\n";
    } else if (c == -2) {
        std::cout << "В таблице нет места\n";
    } else {
        pos = i2 = hash(k2);
        while (pk2[i2].busy > 0) {
            i2 = (i2 + h) % M;
        }
        int d = 0, l = s.length();
        pk1[i1].a = new Item;
        pk1[i1].a->del = d;
        pk1[i1].a->key1 = k1;
        pk1[i1].a->key2 = k2;
        pk1[i1].a->len = l;
        offset = offset+20;
        pk1[i1].a->off = offset;
        pk1[i1].a->ind1 = i1;
        pk1[i1].a->ind2 = i2;
        std::ofstream f(filename, std::ios::binary | std::ios::app);
        f.write((char*)&d, sizeof(int));
        f.write((char*)&k1, sizeof(int));
        f.write((char*)&k2, sizeof(int));
        f.write((char*)&l, sizeof(int));
        f.write((char*)&offset, sizeof(int));
        f.write(s.c_str(), pk1[i1].a->len);
        f.close();
        pk2[i2].a = pk1[i1].a;
        pk1[i1].busy = pk2[i1].busy = 1;
        size += 1;
        offset = offset+l;
    }
}

void Table::del(int k, int number, const std::string& filename) {
    int pos, i, c = 0;
    pos = i = hash(k);
    int d = 1;
    if (number == 1) {
        while (true) {
            if (pk1[i].busy > 0 && pk1[i].a->key1 == k) {
                int ind = pk1[i].a->ind2;
                std::fstream f(filename, std::ios::binary | std::ios::in | std::ios::out);
                f.seekg(pk1[i].a->off-20, std::ios_base::beg);
                f.write((char*)&d, sizeof(int));
                f.close();
                c = 1;
            }
            i = (i + h) % M;
            if (i == pos)
                break;
        }
        if (c == 0)
            std::cout << "Элементов с таким ключом нет\n";
    }
    if (number == 2) {
        while (true) {
            if (pk1[i].busy > 0 && pk1[i].a->key1 == k) {
                int ind = pk1[i].a->ind2;
                std::fstream f(filename, std::ios::binary | std::ios::in | std::ios::out);
                f.seekg(pk1[i].a->off-20, std::ios_base::beg);
                f.write((char*)&d, sizeof(int));
                f.close();
                c = 1;
            }
            i = (i + h) % M;
            if (i == pos)
                break;
        }
        if (c == 0)
            std::cout << "Элементов с таким ключом нет\n";
    }
}

void Table::find(int k, int number, const std::string& filename) {
    int pos, i, c = 0;
    pos = i = hash(k);
    if (number == 1) {
        while(true) {
            if (pk1[i].busy > 0 && pk1[i].a->key1 == k) {
                if (c == 0) {
                    std::cout << "Найденные элементы:\n";
                }
                std::ifstream f(filename, std::ios::binary | std::ios::in);
                f.seekg(pk1[i].a->off, std::ios_base::beg);
                char *buffer = new char[pk1[i].a->len + 1];
                f.read(buffer, pk1[i].a->len);
                buffer[pk1[i].a->len] = 0;
                std::string str = buffer;
                std::cout << pk1[i].a->key1 << ' ' << pk1[i].a->key2 << ' ' << str << '\n';
                c = 1;
                f.close();
                delete[] buffer;
            }
            i = (i+h) % M;
            if (i == pos)
                break;
        }
        if (c == 0)
            std::cout << "Элементов с таким ключом нет\n";
    }
    if (number == 2) {
        while(true) {
            if (pk1[i].busy > 0 && pk1[i].a->key1 == k) {
                if (c == 0) {
                    std::cout << "Найденные элементы:\n";
                }
                std::ifstream f(filename, std::ios::binary | std::ios::in);
                f.seekg(pk1[i].a->off, std::ios_base::beg);
                char *buffer = new char[pk1[i].a->len + 1];
                f.read(buffer, pk1[i].a->len);
                buffer[pk1[i].a->len] = 0;
                std::string str = buffer;
                std::cout << pk1[i].a->key1 << ' ' << pk1[i].a->key2 << ' ' << str << '\n';
                c = 1;
                f.close();
                delete[] buffer;
            }
            i = (i+h) % M;
            if (i == pos) {
                break;
            }
        }
        if (c == 0) {
            std::cout << "Элементов с таким ключом нет\n";
        }
    }
}

int Table::hash(int k) {
    return k % M;
}

void Table::show(const std::string& filename) const {
    int c = 0;
    std::ifstream f(filename, std::ios::binary | std::ios::in);
    while (true) {
        int d = 0;
        int k1 = 0;
        int k2 = 0;
        int l = 0;
        int of = 0;
        f.read((char *) &d, sizeof(int));
        if (f.eof()) {
            break;
        }
        f.read((char *) &k1, sizeof(int));
        f.read((char *) &k2, sizeof(int));
        f.read((char *) &l, sizeof(int));
        f.read((char *) &of, sizeof(int));
        char *buffer = new char[l + 1];
        f.read(buffer, l);
        buffer[l] = 0;
        std::cout << d << " " << k1 << " " << k2 << " " << buffer << "\n";
        delete[] buffer;
        c = 1;
    }
    f.close();
    if (c == 0) {
        std::cout << "Таблица пустая\n";
    }
}

void Table::update(const std::string& filename) {
    std::ifstream f(filename, std::ios::binary | std::ios::in);
    std::ofstream f_("ff", std::ios::binary | std::ios::out);
    while (true) {
        int d = 0;
        int k1 = 0;
        int k2 = 0;
        int l = 0;
        int of = 0;
        f.read((char *) &d, sizeof(int));
        if (f.eof()) {
            break;
        }        std::cout << d << " " << k1 << " " << k2 << " " << buffer << "\n";

        f.read((char *) &k1, sizeof(int));
        f.read((char *) &k2, sizeof(int));
        f.read((char *) &l, sizeof(int));
        f.read((char *) &of, sizeof(int));
        char *buffer = new char[l + 1];
        f.read(buffer, l);
        buffer[l] = 0;
        if (d == 0) {
            f_.write((char *) &d, sizeof(int));
            f_.write((char *) &k1, sizeof(int));
            f_.write((char *) &k2, sizeof(int));
            f_.write((char *) &l, sizeof(int));
            f_.write((char *) &of, sizeof(int));
            f_.write(buffer, l);
        }
        delete[] buffer;
    }
    f.close();
    f_.close();
}

void Table::add_from_file(int d, int k1, int k2, int l, int off) {
    int pos, i1, i2;
    int c = 0;
    pos = i1 = hash(k1);
    while (pk1[i1].busy > 0) {
        i1 = (i1 + h) % M;
        if (i1 == pos) {
            c = -2;
            break;
        }
    }
    if (c == -2) {
        std::cout << "В таблице нет места\n";
    } else {
        pos = i2 = hash(k2);
        while (pk2[i2].busy > 0) {
            i2 = (i2 + h) % M;
        }
        pk1[i1].a = new Item;
        pk1[i1].a->del = d;
        pk1[i1].a->key1 = k1;
        pk1[i1].a->key2 = k2;
        pk1[i1].a->len = l;
        pk1[i1].a->off = off;
        pk1[i1].a->ind1 = i1;
        pk1[i1].a->ind2 = i2;
        pk2[i2].a = pk1[i1].a;
        pk1[i1].busy = pk2[i1].busy = 1;
        size += 1;
        offset = off+l;
        std::cout << offset << "\n";
    }
}
