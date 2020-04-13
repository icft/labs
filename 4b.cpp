#include <iostream>
#include <string>
#include <fstream>

using namespace std;

#pragma pack (push, 1)
struct Item {
    int key1;
    int key2;
    int len;
    string str;
    int ind1;
    int ind2;
};
#pragma pack (pop)

int size = 0;
const int M = 31;
const int h = 3;

struct Space {
    Item *a;
    int busy; // 0-свободна, 1-занята, (-1)-удалена
    Space() {
        a = nullptr;
        busy = 0;
    }
};

class Table {
private:
    Space pk1[M];
    Space pk2[M];
public:
    ~Table();
    int load_from_file();
    void add(int k1, int k2, const string& s, int flag);
    void del(int k, int number);
    void find(int k, int number);
    static int hash(int k);
    void show();
    void upload_to_file();
};

Table::~Table() {
    for(auto & i : pk1)
    {
        delete i.a;
    }
}

int Table::load_from_file() {
    cout << "Введите имя файла: ";
    string filename;
    cin >> filename;
    ifstream f(filename, ios::in | ios::binary);
    if (!f.is_open()) {
        cout << "Невозможно открыть файл\n";
    }
    else {
        while (true) {
            if (size == M) {
                size = 0;
                f.close();
                return 0;
            }
            int c;
            int k1 = 0, k2 = 0, l = 0, i1 = 0, i2 = 0;
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


void Table::add(int k1, int k2, const string& s, int flag) {
    int pos, i1, i2, c = 0, fg = 1;
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
    if ((c == -1) & (flag == 1))
        cout << "В таблице уже есть элемент с такими ключами\n";
    else if ((c == -1) & (flag == 0))
        fg = 0;
    else if (c == -2)
        cout << "В таблице нет места\n";
    else if (fg) {
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
            cout << "Нет элемента с таким ключом!\n";
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
            cout << "Нет элемента с таким ключом!\n";
    }
}

void Table::find(int k, int number) {
    int pos, i, c = 0;
    pos = i = hash(k);
    if (number == 1) {
        while(true) {
            if (pk1[i].busy > 0 && pk1[i].a->key1 == k) {
                if (c == 0)
                    cout << "Найденные элементы:\n";
                cout << pk1[i].a->key1 << ' ' << pk1[i].a->key2 << ' ' << pk1[i].a->str << '\n';
                c = 1;
            }
            i = (i+h) % M;
            if (i == pos)
                break;
        }
        if (c == 0)
            cout << "Элементов с таким ключом нет\n";
    }
    if (number == 2) {
        while(true) {
            if (pk1[i].busy > 0 && pk1[i].a->key1 == k) {
                if (c == 0)
                    cout << "Найденные элементы:\n";
                cout << pk1[i].a->key1 << ' ' << pk1[i].a->key2 << ' ' << pk1[i].a->str << '\n';
                c = 1;
            }
            i = (i+h) % M;
            if (i == pos)
                break;
        }
        if (c == 0)
            cout << "Элементов с таким ключом нет\n";
    }
}

int Table::hash(int k) {
    return k % M;
}

void Table::show() {
    int c = 0;
    for(auto & i : pk1) {
        if (i.busy == 1) {
            cout << i.a->key1 << ' ' << i.a->key2 << ' ' << i.a->str << "\n";
            c = 1;
        }
    }
    if (c == 0)
        cout << "Таблица пустая\n";
}

void Table::upload_to_file() {
    cout << "Введите имя файла: ";
    string filename;
    cin >> filename;
    ofstream f(filename, ios::binary);
    if (!f.is_open())
        cout << "Невозможно открыть файл\n";
    else {
        for(auto & i : pk1) {
            if (i.busy == 1) {
                f.write((char *) &i.a->key1, sizeof(int));
                f.write((char *) &i.a->key2, sizeof(int));
                f.write((char *) &i.a->len, sizeof(int));
                f.write(i.a->str.c_str(), i.a->len);
            }
        }
        f.close();
        cout << "Файл записан!\n";
    }
}

bool check(const string& s) {
    if (s.empty())
        return false;
    for (char i : s)
        if ((i ^ '0') > 9)
            return false;
    return true;
}

int StrToInt(const string& s) {
    int num = 0, i = 0;
    for (int i = 0; i < s.length(); i++){
        num = num*10 + s[i] - 0x30;
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
    if (k == 0)
        cout << "Файл больше допустимого размера\n";
    if (k == 1)
        cout << "Таблица успешно загружена.\n";
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
    }
    else if (!check(k1)) {
        cout << "Первый ключ содержит символы помимо цифр\n";
    }
    else if (!check(k2)) {
        cout << "Второй ключ содержит символы помимо цифр\n";
    }
    else {
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
    }
    else {
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
    }
    else {
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
