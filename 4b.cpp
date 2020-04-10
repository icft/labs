#include <iostream>
#include <string>
#include <fstream>
#include <cstring>

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

int size = 1;
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
    void add(int k1, int k2, const string& s);
    void del(int k, int number);
    void find(int k, int number);
    int hash(int k);
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
    if (!f.is_open())
        cout << "Невозможно открыть файл!\n";
    while(!f.eof()) {
        if (size == M) {
            size = 0;
            f.close();
            return 0;
        }
        pk1[size-1].a = new Item;
        int key1 = pk1[size-1].a->key1, key2 = pk1[size-1].a->key2, len = pk1[size-1].a->len;
        int ind1 = pk1[size-1].a->ind1, ind2 = pk1[size-1].a->ind2;
        f.read((char*)&key1, sizeof(int));
        f.read((char*)&key2, sizeof(int));
        f.read((char*)&len, sizeof(int));
        int strSize = len;
        char *buffer = new char[strSize + 1];
        f.read(buffer, strSize);
        buffer[strSize] = 0;
        pk1[size-1].a->str = buffer;
        delete[] buffer;
        f.read((char*)&ind1, sizeof(int));
        f.read((char*)&ind2, sizeof(int));
        size++;
    }
    f.close();
    return 1;
}


void Table::add(int k1, int k2, const string& s) {
    int pos, i1, i2, c = 0;
    pos = i1 = hash(k1);
    while(pk1[i1].busy > 0)
    {
        if (size == M) {
            break;
        }
        if(pk1[i1].a->key1 == k1 and pk1[i1].a->key2 == k2) {
            c = -1;
            break;
        }
        i1 = (i1+h) % M;
        if(i1 == pos) {
            c = -2;
            break;
        }
    }
    if (c == -1)
        cout << "В таблице уже есть элемент с такими ключами!\n";
    else if (c == -2)
        cout << "В таблице нет места!\n";
    else {
        pos = i2 = hash(k2);
        while(pk2[i2].busy > 0) {
            i2 = (i2+h) % M;
        }
        pk1[i1].a = new Item;
        pk1[i1].a->key1 = k1;
        pk1[i1].a->key2 = k2;
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
            cout << "Нет элемента с таким ключом!";
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
            cout << "Нет элемента с таким ключом!";
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
        cout << "Таблица пустая!\n";
}

void Table::upload_to_file() {
    cout << "Введите имя файла: ";
    string filename;
    cin >> filename;
    ofstream f(filename, ios::in | ios::binary);
    if (!f.is_open())
        cout << "Невозможно открыть файл";
    while(!f.eof()) {
        int key1 = pk1[size-1].a->key1, key2 = pk1[size-1].a->key2, len = pk1[size-1].a->len;
        int ind1 = pk1[size-1].a->ind1, ind2 = pk1[size-1].a->ind2;
        f.write((char*)&key1, sizeof(int));
        f.write((char*)&key2, sizeof(int));
        f.write((char*)&len, sizeof(int));
        const char *str = pk1[size-1].a->str.c_str();
        f.write(str, len);
        f.write((char*)&ind1, sizeof(int));
        f.write((char*)&ind2, sizeof(int));
    }
    f.close();
}

void Menu_text() {
    cout << "1. Загрузить таблицу в память\n";
    cout << "2. Добавить элемент\n";
    cout << "3. Поиск элемента\n";
    cout << "4. Удалить элемент\n";
    cout << "5. Вывести таблицу\n";
    cout << "6. Загрузить таблицу в файл\n";
    cout << "7. Выход\n";
}

void load_from_file(Table *tbl){
    int k = tbl->load_from_file();
    if (k == 0)
        cout << "Файл больше допустимого размера!\n";
    if (k == 1)
        cout << "Таблица успешно загружена.\n";
}

void add_menu(Table *tbl) {
    int key1, key2;
    string s;
    cout << "Введите ключи и информацию: ";
    cin >> key1 >> key2 >> s;
    tbl->add(key1, key2, s);
}

void find_menu(Table *tbl) {
    int k, number;
    cout << "Введите номер пространства: ";
    cin >> number;
    cout << "Введите ключ: ";
    cin >> k;
    tbl->find(k, number);
}

void del_menu(Table *tbl) {
    int k, number;
    cout << "Введите номер пространства: ";
    cin >> number;
    cout << "Введите ключ: ";
    cin >> k;
    tbl->del(k, number);
}

void show_menu(Table *tbl) {
    tbl->show();
}

int main(){
    Table *tbl = new Table();
    int c = 3;
    Menu_text();
    while(c != 7) {
        cout << "Выберите команду: ";
        cin >> c;
        switch (c) {
            case 1:
                load_from_file(tbl);
                break;
            case 2:
                add_menu(tbl);
                break;
            case 3:
                find_menu(tbl);
                break;
            case 4:
                del_menu(tbl);
                break;
            case 5:
                show_menu(tbl);
                break;
            case 6:
                tbl->upload_to_file();
                break;
        }
    }
    delete tbl;
}
