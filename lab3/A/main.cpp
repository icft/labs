#include <iostream>
#include "Scheme.h"
#include <list>

void Menu() {
    std::cout << "1.Переопределить состояния клемм\n"
        "2.Вывод\n"
        "3.Установить состояние клеммы по номеру\n"
        "4.Вывести текущее состояние клеммы по номеру\n"
        "5.Увеличить количество соединений для клеммы по номеру\n"
        "6.Уменьшить количество соединений для клеммы по номеру\n"
        "7.Добавить клемму\n"
        "8.Выход\n";
}

void Create() {
    std::cout << "Варианты создания элемента:\n"
        "1.Ввод количества входных и выходных клемм. В данном случае вначале будут входные, а потом выходные клеммы. Их номера будут совпадать их индексам\n"
        "2.Ввод клемм\n"
        "Введите тип ввода: ";
}

int main() {
    setlocale(LC_ALL, "RUSSIAN");
    Create();
    int c;
    Scheme sc;
    std::cin >> c;
    if (c == 1) {
        std::cout << "Количество входных и выходных клемм: ";
        int inp, out;
        std::cin >> inp >> out;
        sc.init_count(inp, out);
    }
    else {
        std::list<struct clem*> list;
        int size = 0;
        std::cout << "Ввод клемм. Номер, тип(0-входная, 1-выходная), количество соединений, тип сигнала\n";
        while (size < MAX_SIZE) {
            struct clem* d = new struct clem;
            int num, type, count;
            char s;
            std::cin >> num >> type >> count >> s;
            if (num == -1)
                break;
            if ((type == 0) && (count > 1)) {
                std::cout << "Количество соединений для входной клемы должно быть меньше 2\n";
                continue;
            }
            if ((type == 1) && (count > 4)) {
                std::cout << "Количество соединений для входной клемы должно быть меньше 4\n";
                continue;
            }
            if ((s != '0') && (s != '1') && (s != 'x')) {
                std::cout << "Такого типа сигнала не существует\n";
                continue;
            }
            d->number = num, d->type = type, d->count = count, d->signal = s;
            list.push_back(d);
            size++;

        }
        sc.init_arr(list);
    }
    Menu();
    c = -1;
    while (c != 8) {
        std::cout << "Введите номер команды: ";
        std::cin >> c;
        switch (c) {
        case 1: {
            sc.overriding_states();
            break;
        }
        case 2: {
            sc.print();
            break;
        }
        case 3: {
            std::cout << "Введите номер клеммы: ";
            int num;
            std::cin >> num;
            sc.set_new_state(num);
            break;
        }
        case 4: {
            std::cout << "Введите номер клеммы: ";
            int num;
            std::cin >> num;
            char c = sc.get_state(num);
            break;
        }
        case 5: {
            std::cout << "Введите номер клеммы: ";
            int num;
            std::cin >> num;
            sc.add_clem_connection(num);
            break;
        } 
        case 6: {
            std::cout << "Введите номер клеммы: ";
            int num;
            std::cin >> num;
            sc.reduce_clem_connection(num);
            break;
        }
        case 7: {
            struct clem* f = new struct clem;
            std::cout << "Введите данные клеммы(номер, тип, количество, сигнал): ";
            std::cin >> f->number >> f->type >> f->count >> f->signal;
            if ((f->type == 0) && (f->count > 1)) {
                std::cout << "Количество соединений для входной клемы должно быть меньше 2\n";
                delete f;
            }
            if ((f->type == 1) && (f->count > 4)) {
                std::cout << "Количество соединений для входной клемы должно быть меньше 4\n";
                delete f;
            }
            if ((f->signal != '0') && (f->signal != '1') && (f->signal != 'x')) {
                std::cout << "Такого типа сигнала не существует\n";
                delete f;
            }
            sc.add_clem(f);
            break;
        }
        }
    }
}
