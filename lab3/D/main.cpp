#include <iostream>
#include "tablef.h"
#include <climits>

inline int A(double a, double b)
{
    return a > b;
}

inline int B(double a, double b)
{
    return a < b;
}

double funс(double x) {
    return x * x;
}

void add(function& table) {
    int count;
    std::cout << "Введите количество элементов: ";
    std::cin >> count;
    for (int i = 0; i < count; i++) {
        double x, y;
        std::cout << "Введите координаты точки: ";
        std::cin >> x >> y;
        int c = table.SetPoint(x, y);
        if (c == -1) {
            std::cout << "Таблица заполнена\n";
            return;
        }
        if (c == -2) {
            std::cout << "Такая точка уже существует\n";
            return;
        }
    }
}

void min_max(function& table) {
    std::cout << "Максимум: " << table.GetMinMax(&A) << "\n" << "Минимум: " << table.GetMinMax(&B) << "\n";
}

void type(function& table) {
    int c = table.GetType();
    if (c == -3)
        std::cout << "Таблица пустая\n";
    else if (c == 0)
        std::cout << "Функция постоянна\n";
    else if (c == 1)
        std::cout << "Возрастающая функция\n";
    else if (c == -1)
        std::cout << "Убывающая функция\n";
    else if (c == -2)
        std::cout << "Обычная функция\n";
}

void get_value(function& table) {
    double x, y;
    std::cout << "Введите абсциссу точки: ";
    std::cin >> x;
    y = table.GetInterpolation(x);
    std::cout << "Ордината точки: " << y << "\n";
}

void menu() {
    std::cout << "1. Добавить точки\n";
    std::cout << "2. Получить абсциссу точки по её ординате\n";
    std::cout << "3. Вывести тип функции\n";
    std::cout << "4. Минимум и максимум функции\n";
    std::cout << "5. Вывод таблицы\n";
    std::cout << "6. Выход\n";
}

int main() {
    setlocale(LC_ALL, "Russian");
    std::cout << "Создание функции... \n";
    std::cout << "1. Ввод по 1 точке\n";
    std::cout << "2. Ввод массивом максимального размера\n";
    int c;
    std::cout << "Введите номер: ";
    std::cin >> c;
    function* f = nullptr;
    if (c == 1) {
        std::cout << "Введите координаты точки: ";
        double x, y;
        std::cin >> x >> y;
        f = new function(x, y, 1);
    } if (c == 2) {
        int size = 5;
        double* arr = new double[size];
        std::cout << "Ввод точек\n";
        for (int i = 0; i < size; i++) {
            std::cin >> arr[i];
        }
        f = new function(arr, funс);
    }
    menu();
    while (c != 6) {
        std::cout << "Введите команду: ";
        std::cin >> c;
        switch (c) {
        case 1:
            add(*f);
            break;
        case 2:
            get_value(*f);
            break;
        case 3:
            type(*f);
            break;
        case 4:
            min_max(*f);
            break;
        case 5:
            f->GetTable();
            break;
        }
    }
}

