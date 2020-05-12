#include <iostream>
#include <cstdlib>
#include <string>
#include "btree.h"
#include <fstream>

using namespace std;

const int M = 200;
int size;

void menu () {
    cout << "1.Вставить элемент\n";
    cout << "2.Удалить элемент\n";
    cout << "3.Поиск\n";
    cout << "4.Вывести дерево\n";
    cout << "5.Вывести элемент следующий за заданным\n";
    cout << "7.Выход\n";
}

void Insert(btree** T) {
    info n;
    int k;
    string s;
    cout << "Введите ключ: ";
    cin >> k;
    cout << "Введите информацию: ";
    cin >> s;
    n.key = k;
    n.s = s;
    *T = insert(*T, n);
}

void Delete(btree** T) {
    int k;
    cout << "Введите ключ: ";
    cin >> k;
    *T = remove(*T, k);
}

void Search(btree* T) {
    int k;
    cout << "Введите ключ: ";
    cin >> k;
    search(T, k);
}

void Sled(btree* T) {
    int k;
    cout << "Введите ключ: ";
    cin >> k;
    sled(T, k);
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
                Sled(T);
                break;
            case 6:
                inorder(T, 0);
                break;
        }
    }
}
