#include "table.h"
#include <fstream>
#include <string>
#include <iostream>

Table::~Table() {
    for(auto & i : pk1)
    {
        delete i.a;
    }
}

const int h = 3;
int size = 0;

int Table::load_from_file() {
    std::cout << "Введите имя файла: ";
    std::string filename;
    std::cin >> filename;
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
            int c;
            int k1 = 0;
            int k2 = 0;
            int l = 0;
            int i1 = 0;
            int i2 = 0;
            f.read((char *)&k1, sizeof(int));
            if (f.eof()) {
                break;
            }
            f.read((char *)&k2, sizeof(int));
            f.read((char *)&l, sizeof(int));
            char *buffer = new char[l + 1];
            f.read(buffer, l);
            buffer[l] = 0;
            add(k1, k2, buffer, 0);
            delete[] buffer;
        }
        f.close();
        return 1;
    }
}

void Table::add(int k1, int k2, const std::string& s, int flag) {
    int pos, i1, i2;
    int c = 0;
    int fg = 1;
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
    if ((c == -1) & (flag == 1)) {
        std::cout << "В таблице уже есть элемент с такими ключами\n";
    } else if ((c == -1) & (flag == 0)) {
        fg = 0;
    } else if (c == -2) {
        std::cout << "В таблице нет места\n";
    } else if (fg) {
        pos = i2 = hash(k2);
        while (pk2[i2].busy > 0) {
            i2 = (i2 + h) % M;
        }
        pk1[i1].a = new Item;
        pk1[i1].a->key1 = k1;
        pk1[i1].a->key2 = k2;
        pk1[i1].a->len = s.length();
        pk1[i1].a->str = s;
        pk1[i1].a->ind1 = i1;
        pk1[i1].a->ind2 = i2;
        pk2[i2].a = pk1[i1].a;
        pk1[i1].busy = pk2[i1].busy = 1;
        size += 1;
    }
}

void Table::del(int k, int number) {
    int pos, i, c = 0;
    pos = i = hash(k);
    if (number == 1) {
        while(true) {
            if (pk1[i].busy > 0 && pk1[i].a->key1 == k) {
                int ind = pk1[i].a->ind2;
                delete pk1[i].a;
                pk1[i].busy = -1;
                pk2[ind].busy = -1;
                c = 1;
            }
            i = (i+h) % M;
            if (i == pos)
                break;
        }
        if (c == 0)
            std::cout << "Нет элемента с таким ключом!\n";
    }
    if (number == 2) {
        while(true) {
            if (pk2[i].busy > 0 && pk2[i].a->key1 == k) {
                int ind = pk2[i].a->ind2;
                delete pk2[i].a;
                pk2[i].busy = -1;
                pk1[ind].busy = -1;
                c = 1;
            }
            i = (i+h) % M;
            if (i == pos)
                break;
        }
        if (c == 0)
            std::cout << "Нет элемента с таким ключом!\n";
    }
}

void Table::find(int k, int number) {
    int pos, i, c = 0;
    pos = i = hash(k);
    if (number == 1) {
        while(true) {
            if (pk1[i].busy > 0 && pk1[i].a->key1 == k) {
                if (c == 0)
                    std::cout << "Найденные элементы:\n";
                std::cout << pk1[i].a->key1 << ' ' << pk1[i].a->key2 << ' ' << pk1[i].a->str << '\n';
                c = 1;
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
                std::cout << pk1[i].a->key1 << ' ' << pk1[i].a->key2 << ' ' << pk1[i].a->str << '\n';
                c = 1;
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

void Table::show() const {
    int c = 0;
    for(auto & i : pk1) {
        if (i.busy == 1) {
            std::cout << i.a->key1 << ' ' << i.a->key2 << ' ' << i.a->str << "\n";
            c = 1;
        }
    }
    if (c == 0) {
        std::cout << "Таблица пустая\n";
    }
}

void Table::upload_to_file() {
    std::cout << "Введите имя файла: ";
    std::string filename;
    std::cin >> filename;
    std::ofstream f(filename, std::ios::binary);
    if (!f.is_open()) {
        std::cout << "Невозможно открыть файл\n";
    } else {
        for(auto & i : pk1) {
            if (i.busy == 1) {
                f.write((char *) &i.a->key1, sizeof(int));
                f.write((char *) &i.a->key2, sizeof(int));
                f.write((char *) &i.a->len, sizeof(int));
                f.write(i.a->str.c_str(), i.a->len);
            }
        }
        f.close();
        std::cout << "Файл записан!\n";
    }
}
