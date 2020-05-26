#include <iostream>
#include <vector>
#include <utility>
#include <climits>
#include <queue>
#include <fstream>
using namespace std;

int size = 0;

typedef struct v_node {
    int number, x, y;
    vector <int> sp;
} node;

int find_num(vector<node*>& m, int num) {
    if (m.empty())
        return -1;
    for (int i = 0; i < m.size(); i++)
        if (m[i]->number == num)
            return i;
    return -1;
}

int find_coordinates(vector<node*>& m, int x, int y) {
    for (auto & i : m)
        if (i->x == x && i->y == y)
            return 1;
    return -1;
}

bool find_edge(vector<node*>& m, int i1, int i2) {
    for (int i : m[i1]->sp)
        if (i == i2)
            return true;
    return false;
}

void add_node(vector<node*>& m, int number, int x, int y) {
    node* n;
    n = new node;
    n->number = number; n->x = x; n->y = y;
    m.push_back(n);
}

void add_edge(vector<node*>& m, int ind1, int ind2) {
    m[ind1]->sp.push_back(ind2);
    m[ind2]->sp.push_back(ind1);
}

void pop(vector<node*>& m, int num) {
    size_t l = m.size(), i;
    if (!find_num(m, num)) {
        cout << "Вершины с таким номером не существует\n";
        return;
    }
    for(i = 0; i < l; i++) {
        if (m[i]->number == num) {
            size_t l_sp = m[i]->sp.size(), j;
            for (j = 0; j < l_sp; j++) {if (!find_num(m, num)) {
                    cout << "Вершины с таким номером не существует\n";
                    return;
                }
                int ind = m[i]->sp[j];
                size_t l_1 = m[ind]->sp.size(), z;
                for (z = 0; z < l_1; z++) {
                    if (m[m[ind]->sp[z]]->number == num) {
                        m[ind]->sp.erase(m[ind]->sp.begin() + z);
                        break;
                    }
                }
            }
            m[i]->sp.clear();
            node* tmp = m[i];
            m.erase(m.begin()+i);
            delete tmp;
            break;
        }
    }
}

void dijkstra(vector<node*>& m, int s) {
    vector<int> d(m.size(), INT_MAX);
    d[s] = 0;
    vector<bool> u(m.size());
    for (int i = 0; i < m.size(); i++) {
        int v = -1;
        for (int j = 0; j < m.size(); j++)
            if (!u[j] && (v == -1 || d[j] < d[v]))
                v = j;
        if (d[v] == INT_MAX) {
            break;
        }
        u[v] = true;
        for (int j = 0; j < m[v]->sp.size(); j++) {
            int t = m[v]->sp[j], len = 1;
            if (d[v] + len < d[t]) {
                d[t] = d[v] + len;
            }
        }
    }
    for(int i = 0; i < m.size(); i++) {
        if (i != s) {
            if (d[i] != INT_MAX)
                cout << "До вершины " << m[i]->number << ": " << d[i] << "\n";
            else
                cout << "До вершины " << m[i]->number << " нет пути\n";
        }
    }
}

void erase(vector<node*>& m) {
    int i, l = m.size();
    while (i <= l) {
        m[0]->sp.clear();
        node* tmp = m[0];
        m.erase(m.begin());
        delete tmp;
        i++;
    }
}

void generate(vector<node*>& m, int v, int e) {
    srand(time(nullptr));
    if (e > (v * (v - 1) / 2)) {
        cout << "Граф с такими параметрами создать невозможно\n";
        return;
    }
    for (int i = 0; i < v;) {
        if (find_num(m, i+1) == 0) {
            cout << i << "\n";
            int x = (rand() % v) + 1;
            int y = (rand() % v) + 1;
            add_node(m, i+1, x, y);
            i++;
        }
    }
    for (int i = 0; i < e;) {
        int a = rand() & v + 1, b = rand() & v + 1;
        if (find_num(m, a) == 1 && find_num(m, b) == 1) {
            add_edge(m, a, b);
            i++;
        }
    }
}

void Menu() {
    cout << "1.Добавить вершину\n";
    cout << "2.Добавить ребро\n";
    cout << "3.Удалить вершину\n";
    cout << "4.Кратчайшее расстояние\n";
    cout << "5.Вывод\n";
    cout << "6.Сгенерировать\n";
    cout << "7.Загрузить из файла\n";
    cout << "8.Записать в файл\n";
    cout << "9.Выход\n";
}

void READ_FILE(vector<node*>& m) {
    string name;
    cout << "Введите имя файла: ";
    cin >> name;
    ifstream f(name, ios::in | ios::binary);
    if (!f.is_open()) {
        cout << "Файл открыть невозможно\n";
    } else {
        int i, c;
        f.read((char *)&c, sizeof(int));
        while (i != c) {
            int n = 0;
            int x = 0;
            int y = 0;
            int l = 0;
            int e = 0;
            f.read((char *)&n, sizeof(int));
            if (f.eof()) {
                break;
            }
            f.read((char *)&x, sizeof(int));
            f.read((char *)&y, sizeof(int));
            f.read((char *)&l, sizeof(int));
            add_node(m, n, x, y);
            f.seekg(l * sizeof(int), ios_base::cur);
            i++;
        }
        f.seekg(0, ios_base::beg);
        i = 0;
        while (i != c) {
            f.seekg(sizeof(int), ios_base::cur);
            int n = 0;
            int l = 0;
            int e = 0;
            f.read((char *)&n, sizeof(int));
            if (f.eof()) {
                break;
            }
            f.seekg(2*sizeof(int), ios_base::cur);
            f.read((char *)&l, sizeof(int));
            for (int j = 0; i < l; i++) {
                f.read((char*)&e, sizeof(int));
                add_edge(m, n, e);
            }
            i++;
        }
        f.close();
    }
}

void WRITE_FILE(vector<node*>& m) {
    string name;
    cout << "Введите имя файла: ";
    cin >> name;
    ofstream f(name, ios::out | ios::binary);
    if (!f.is_open())
        cout << "Файл открыть невозможно\n";
    else {
        int l = m.size(), l1;
        f.write((char*)&l, sizeof(int));
        for (int i = 0; i < l; i++) {
            f.write((char*)&m[i]->number, sizeof(int));
            f.write((char*)&m[i]->x, sizeof(int));
            f.write((char*)&m[i]->y, sizeof(int));
            l1 = m[i]->sp.size();
            f.write((char*)&l1, sizeof(int));
            for (int j = 0; j < l1; j++) {
                f.write((char*)&(m[m[i]->sp[j]]->number), sizeof(int));
            }
        }
    }
}

void ADD_NODE(vector<node*>& m) {
    int number, x, y;
    cout << "Введите номер и координаты вершины: ";
    cin >> number >> x >> y;
    if (find_num(m , number) >= 0) {
        cout << "Вершина с таким номером существует\n";
        return;
    }
    if (find_coordinates(m, x, y) >= 0) {
        cout << "Вершина с такими координатами существует\n";
        return;
    }
    add_node(m, number, x ,y);
}

void ADD_EDGE(vector<node*>& m) {
    int num1, num2;
    cout << "Введите номера вершин: ";
    cin >> num1 >> num2;
    int a = find_num(m, num1), b = find_num(m, num2);
    if (a < 0) {
        cout << "Вершины с номером " << num1 << " не существует\n";
        return;
    } else if (b < 0) {
        cout << "Вершины с номером " << num2 << " не существует\n";
        return;
    } else if (find_edge(m, num1, num2)) {
        cout << "Такое ребро существует\n";
        return;
    } else if (m.size() == 1) {
        cout << "В графе всего 1 вершина\n";
        return;
    }
    add_edge(m, a, b);
}

void DEL(vector<node*>& m) {
    int num;
    cout << "Введите номер вершины: ";
    cin >> num;
    pop(m , num);
}

void DIJKSTRA(vector<node*>& m) {
    int num;
    cout << "Введите номер вершины: ";
    cin >> num;
    int a = find_num(m , num);
    if (a < 0) {
        cout << "Вершины с таким номером не существует\n";
        return;
    }
    dijkstra(m , a);
}

void GENERATE(vector<node*>& m) {
    int v, e;
    cout << "Введите количество вершин и ребер: ";
    cin >> v >> e;
    generate(m, v, e);
}

void PRINT(vector<node*>& m) {
    if (m.empty()) {
        cout << "Граф пуст\n";
        return;
    }
    size_t l = m.size(), i;
    for (i = 0; i < l; i++) {
        cout << m[i]->number << ":" << " ";
        size_t l_sp = m[i]->sp.size(), j;
        for (j = 0; j < l_sp; j++)
            cout << m[m[i]->sp[j]]->number << " ";
        cout << "\n";
    }
}

int main() {
    vector<node*> m;
    Menu();
    int c = 0;
    while (c != 9) {
        cout << "Введите команду: ";
        cin >> c;
        switch (c) {
            case 1:
                ADD_NODE(m);
                break;
            case 2:
                ADD_EDGE(m);
                break;
            case 3:
                DEL(m);
                break;
            case 4:
                DIJKSTRA(m);
                break;
            case 5:
                PRINT(m);
                break;
            case 6:
                GENERATE(m);
                break;
            case 7:
                READ_FILE(m);
                break;
            case 8:
                WRITE_FILE(m);
                break;
        }
    }
}
