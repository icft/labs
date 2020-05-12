#include <iostream>
#include <cstdlib>
#include <string>
#include "btree.h"
#include <fstream>

using namespace std;


bool check(const string& s) {
    if (s.empty()) {
        return false;
    }
    for (char i : s) {
        if ((i ^ '0') > 9) {
            return false;
        }
    }
    return true;
}

int StrToInt(const string& s) {
    int num = 0, i = 0;
    for (char i : s) {
        num = num*10 + i - 0x30;
    }
    return num;
}

void menu () {
    cout << "1.Вставить элемент\n";
    cout << "2.Удалить элемент\n";
    cout << "3.Поиск\n";
    cout << "4.Вывести дерево\n";
    cout << "5.Максимальный элемент\n";
    cout << "6.Вывести в прямом порядке\n";
    cout << "7.Выход\n";
}

void Insert(btree** T) {
    info n;
    string k;
    string s;
    cout << "Введите ключ: ";
    cin >> k;
    cout << "Введите информацию: ";
    cin >> s;
    if (check(k)) {
        n.key = StrToInt(k);
        n.s = s;
        *T = insert(*T, n);
    } else {
        cout << "Ключ содержит символы помимо цифр\n";
    }
}

void Delete(btree** T) {
    string k;
    cout << "Введите ключ: ";
    cin >> k;
    if (check(k)) {
        *T = remove(*T, StrToInt(k));
    } else {
        cout << "Ключ содержит символы помимо цифр\n";
    }
}

void Search(btree* T) {
    string k;
    cout << "Введите ключ: ";
    cin >> k;
    if (check(k)) {
        search(T, StrToInt(k));
    } else {
        cout << "Ключ содержит символы помимо цифр\n";
    }
}

void Max(btree* T) {
    info* n;
    n = max_el(T);
    cout << "Ключ: " << n->key << "\nЗначение: " << n->s << "\n";
}

int main() {
    btree* T = nullptr;
    int c = 1000;
    menu();
    while (c != 7) {
        cout << "Введите номер команды: ";
        cin >> c;
        switch (c) {
            case 1:
                Insert(&T);
                break;
            case 2:
                Delete(&T);
                break;
            case 3:
                Search(T);
                break;
            case 4:
                print(T, 0);
                break;
            case 5:
                Max(T);
                break;
            case 6:
                inorder(T, 0);
                break;
        }
    }
    delete_tree(T);
}
