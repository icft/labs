#include <iostream>
#include <string>
#include "table.h"

using namespace std;

extern int size;
const int h = 3;

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

void Menu_text() {
    cout << "1. Загрузить таблицу из существующего файла память\n";
    cout << "2. Вывести размер таблицы\n";
    cout << "3. Добавить элемент\n";
    cout << "4. Поиск элемента\n";
    cout << "4. Удалить элемент\n";
    cout << "6. Вывести таблицу\n";
    cout << "7. Загрузить таблицу в файл\n";
    cout << "8. Выход\n";
}

void load_from_file(Table *tbl){
    int k = tbl->load_from_file();
    if (k == 1) {
        cout << "Таблица успешно загружена.\n";
    } else if (k == 0) {
        cout << "Файл больше допустимого размера\n";
    } else {
        cout << "Невозможно открыть файл\n";
    }
}

void size_table() {
    cout << "Размер таблицы: " << size << "\n";
}

void add_menu(Table *tbl) {
    string k1, k2, s;
    cout << "Введите ключи и информацию:\nПервый ключ: ";
    cin >> k1;
    cout << "Второй ключ: ";
    cin >> k2;
    cout << "Информация: ";
    cin >> s;
    if (check(k1) && check(k2)) {
        tbl->add(StrToInt(k1), StrToInt(k2), s, 0);
    } else if (!check(k1)) {
        cout << "Первый ключ содержит символы помимо цифр\n";
    } else if (!check(k2)) {
        cout << "Второй ключ содержит символы помимо цифр\n";
    } else {
        cout << "Оба ключа содержвт символы помимо цифр\n";
    }
}

void find_menu(Table *tbl) {
    int number;
    string k;
    cout << "Введите номер пространства: ";
    cin >> number;
    cout << "Введите ключ: ";
    cin >> k;
    if (check(k)) {
        tbl->find(StrToInt(k), number);
    } else {
        cout << "Ключ содержит символы помимо цифр\n";
    }
}

void del_menu(Table *tbl) {
    int number;
    string k;
    cout << "Введите номер пространства: ";
    cin >> number;
    cout << "Введите ключ: ";
    cin >> k;
    if (check(k)) {
        tbl->del(StrToInt(k), number);
    } else {
        cout << "Ключ содержит символы помимо цифр\n";
    }
}

void show_menu(Table *tbl) {
    tbl->show();
}

int main(){
    auto *tbl = new Table();
    int c = 3;
    Menu_text();
    while(c != 8) {
        cout << "Выберите команду: ";
        cin >> c;
        switch (c) {
            case 1:
                load_from_file(tbl);
                break;
            case 2:
                size_table();
                break;
            case 3:
                add_menu(tbl);
                break;
            case 4:
                find_menu(tbl);
                break;
            case 5:
                del_menu(tbl);
                break;
            case 6:
                show_menu(tbl);
                break;
            case 7:
                tbl->upload_to_file();
                break;
        }
    }
    delete tbl;
}
