#include <iostream>
#include "Scheme.h"

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
    Scheme* sc = nullptr;
    std::cin >> c;
    if (c == 1) {
        int inp, out;
        while (true) {
            std::cout << "Количество входных и выходных клемм: ";
            std::cin >> inp >> out;
            try {
                sc = new Scheme(inp, out);
                break;
            }
            catch (std::exception& ex) {
                std::cout << ex.what() << "\n";
                std::cout << "Повторите ввод еще раз\n";
            }
        }
    }
    else {
        int size;
        while (true) {
            std::cout << "Введите количество элементов: ";
            std::cin >> size;
            if (size > MAX_SIZE)
                std::cout << "Размер больше допустимого повтроите ввод\n";
            else
                break;
        }
        struct clem* a = new struct clem[size];
        std::cout << "Ввод клемм. Номер, тип(0-входная, 1-выходная), количество соединений, тип сигнала\n";
        for (int i = 0; i < size; i++) {
            struct clem d;
            std::cin >> d.number >> d.type >> d.count >> d.signal;
            if (d.number == -1)
                break;
            if ((d.type == 0) && (d.count > 1)) {
                std::cout << "Количество соединений для входной клемы должно быть меньше 2\n";
                continue;
            }
            if ((d.type == 1) && (d.count > 4)) {
                std::cout << "Количество соединений для входной клемы должно быть меньше 4\n";
                continue;
            }
            if ((d.signal != '0') && (d.signal != '1') && (d.signal != 'x')) {
                std::cout << "Такого типа сигнала не существует\n";
                continue;
            }
            a[i] = d;
        }
        try {
            sc = new Scheme(a, size);
            delete[] a;
        }
        catch (std::exception& ex) {
            std::cout << ex.what() << "\n";
        }
    }
    Menu();
    c = -1;
    while (c != 8) {
        std::cout << "Введите номер команды: ";
        std::cin >> c;
        switch (c) {
        case 1: {
            std::cout << "Введите новые значения: ";
            std::cin >> *sc;
            break;
        }
        case 2: {
            std::cout << *sc;
            break;
        }
        case 3: {
            std::cout << "Введите номер клеммы и новое состояние: ";
            int num;
            char c;
            std::cin >> num >> c;
            try {
                (*sc)(num, c);
            }
            catch (std::exception& ex) {
                std::cout << ex.what() << "\n";
            }
            break;
        }
        case 4: {
            std::cout << "Введите номер клеммы: ";
            int num;
            std::cin >> num;
            char c;
            try {
                c = (*sc)[num];
                std::cout << "Состояние клеммы: " << c << "\n";
            }
            catch (std::exception& ex) {
                std::cout << ex.what() << "\n";
            }
            break;
        }
        case 5: {
            std::cout << "Введите номер клеммы и количество: ";
            int num, count;
            std::cin >> num >> count;
            try {
                (*sc).add_clem_connection(num, count);
            }
            catch (std::exception& ex) {
                std::cout << ex.what() << "\n";
            }
            break;
        }
        case 6: {
            std::cout << "Введите номер клеммы и количество: ";
            int num, count;
            std::cin >> num >> count;
            try {
                (*sc).reduce_clem_connection(num, count);
            }
            catch (std::exception& ex) {
                std::cout << ex.what() << "\n";
            }
            break;
        }
        case 7: {
            struct clem f;
            std::cout << "Введите данные клеммы(номер, тип, количество, сигнал): ";
            std::cin >> f.number >> f.type >> f.count >> f.signal;
            if ((f.type == 0) && (f.count > 1)) {
                std::cout << "Количество соединений для входной клемы должно быть меньше 2\n";
            }
            if ((f.type == 1) && (f.count > 4)) {
                std::cout << "Количество соединений для входной клемы должно быть меньше 4\n";
            }
            if ((f.signal != '0') && (f.signal != '1') && (f.signal != 'x')) {
                std::cout << "Такого типа сигнала не существует\n";
            }
            try {
                *sc += f;
            }
            catch (std::exception& ex) {
                std::cout << ex.what() << "\n";
            }
            break;
        }
        }
    }
}
