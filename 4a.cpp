#include <iostream>
#include <string.h>

using namespace std;

struct Item{
    int key1;
    int key2;
    string str;
    int ind1;
    int ind2;
};

const int M = 31;

struct Space{
    Item *a;
    int busy; // 0-свободна, 1-занята, (-1)-удалена
    Space()
    {
        a = nullptr;
        busy = 0;
    }
};

class Table
{
    Space pk1[M];
    Space pk2[M];
public:
    ~Table();
    void add(int k1, int k2, string s);
    void del(int k, int number);
    void find(int k, int number);
    int hash(int k);
    void show() const;
};

Table::~Table()
{
    for(int i = 0; i < M; i++)
    {
        delete pk1[i].a;
    }
}

int Table::hash(int k)
{
    return k % M;
}

void Table::add(int k1, int k2, string s)
{
    int h = 3;
    int pos, i1, i2, c = 0;
    pos = i1 = hash(k1);
    while(pk1[i1].busy > 0)
    {
        if(pk1[i1].a->key1 == k1 and pk1[i1].a->key2 == k2)
        {
            c = -1;
            break;
        }
        i1 = (i1+h) % M;
        if(i1 == pos)
        {
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
        while(pk2[i2].busy > 0)
        {
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
        cout << pk1[i1].a->key1 << ' ' << pk1[i1].a->key2 << ' ' << pk1[i1].a->str << '\n';
    }
}

void Table::del(int k, int number)
{
    int h = 3;
    int pos, i, c = 0;
    pos = i = hash(k);
    if (number == 1)
    {
        while(true)
        {
            if (pk1[i].busy > 0 && pk1[i].a->key1 == k)
            {
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
    if (number == 2)
    {
        while(true)
        {
            if (pk2[i].busy > 0 && pk2[i].a->key1 == k)
            {
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

void Table::find(int k, int number)
{
    int h = 3;
    int pos, i, c = 0;
    pos = i = hash(k);
    if (number == 1)
    {
        while(true)
        {
            if (pk1[i].busy > 0 && pk1[i].a->key1 == k)
            {
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
    if (number == 2)
    {
        while(true)
        {
            if (pk1[i].busy > 0 && pk1[i].a->key1 == k)
            {
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

void Table::show() const
{
    int c = 0;
    for(int i = 0; i < M; i++)
    {
        if (pk1[i].busy == 1)
        {
            cout << pk1[i].a->key1 << ' ' << pk1[i].a->key2 << ' ' << pk1[i].a->str << "\n";
            c = 1;
        }
    }
    if (c == 0)
        cout << "Таблица пустая!\n";
}

void Menu_text()
{
    cout << "1. Добавить элемент\n";
    cout << "2. Поиск элемента\n";
    cout << "3. Удалить элемент\n";
    cout << "4. Вывести таблицу\n";
    cout << "5. Выход\n";
    cout << "Выберите команду: ";
}

void add_menu(Table *tbl)
{
    int key1, key2;
    string s;
    cout << "Введите ключи и информацию: ";
    cin >> key1 >> key2 >> s;
    tbl->add(key1, key2, s);
}

void find_menu(Table *tbl)
{
    int k, number;
    cout << "Введите номер пространства: ";
    cin >> number;
    cout << "Введите ключ: ";
    cin >> k;
    tbl->find(k, number);
}

void del_menu(Table *tbl)
{
    int k, number;
    cout << "Введите номер пространства: ";
    cin >> number;
    cout << "Введите ключ: ";
    cin >> k;
    tbl->del(k, number);
}

void show_menu(Table *tbl)
{
    tbl->show();
}

int main()
{
    Table *tbl = new Table();
    int c = 3;
    while(c != 5)
    {
        Menu_text();
        cin >> c;
        switch (c) {
        case 1:
            add_menu(tbl);
            break;
        case 2:
            find_menu(tbl);
            break;
        case 3:
            del_menu(tbl);
            break;
        case 4:
            show_menu(tbl);
        }
    }
    delete tbl;
}
