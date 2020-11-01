#include "Scheme.h"
#include <string>
#include <iostream>

void Scheme::init_count(int in, int ou) {
    if (in + ou > MAX_SIZE)
        throw "Превышено максимальное количество клемм";
    amount = in + ou;
    for (int i = 0; i < in; i++) {
        arr[i] = new struct clem;
        arr[i]->number = i + 1;
        arr[i]->type = 1;
        arr[i]->count = 0;
        arr[i]->signal = 'x';
    }
    for (int i = in; i < amount; i++) {
        arr[i] = new struct clem;
        arr[i]->number = i + 1;
        arr[i]->type = 0;
        arr[i]->count = 0;
        arr[i]->signal = 'x';
    }
}

void Scheme::init_arr(const std::list<struct clem*> list) {
    if (list.size() > MAX_SIZE)
        throw "Превышено максимальное количество клемм";
    for (const auto& it : list) {
        arr[amount] = new struct clem;
        arr[amount] = it;
        amount++;
    }
}

void Scheme::overriding_states() {
    int i = 0;
    while (i < amount) {
        std::cout << "Для клемы " << i + 1 << " новое состояние: ";
        std::cin >> arr[i]->signal;
        i++;
    }
}

int Scheme::find(int num) {
    for (int i = 0; i < amount; i++) {
        if (arr[i]->number == num)
            return i;
    }
    return -1;
}

void Scheme::print() {
    for (int i = 0; i < amount; i++) {
        std::string type, state;
        type = (arr[i]->type == 0) ? "входная" : "выходная";
        std::cout << "Номер: " << arr[i]->number << ";Тип: " << type << ";Количество соединений: " << arr[i]->count << ";Тип сигнала: " << arr[i]->signal << "\n";
    }
}

void Scheme::set_new_state(int num) {
    int i = find(num);
    if (i > -1) {
        std::cout << "Новое значение: ";
        std::cin >> arr[i]->signal;
    }
    else
        throw "Нет клеммы с таким номером";
}

char Scheme::get_state(int num) {
    int i = find(num);
    if (i > -1) {
        return arr[i]->signal;
    }
    else
        throw "Нет клеммы с таким номером";
}

void Scheme::add_clem_connection(int num) {
    int i = find(num);
    if (i < 0)
        throw "Нет клеммы с таким номером";
    std::cout << "Введите новое количество соединений для клеммы: ";
    int c;
    std::cin >> c;
    std::string f = (arr[i]->type == 0) ? "входной" : "выходной";
    if ((f == "входной" && c > 1) or (f == "выходной" && c > 3))
        throw "Превышено максимальное число соединений для " + f + " клеммы";
    arr[i]->count = c;
}

void Scheme::reduce_clem_connection(int num) {
    int i = find(num);
    if (i < 0)
        throw "Нет клеммы с таким номером";
    if (arr[i]->type == 0) {
        std::cout << "Введите новое количество соединений для входной клеммы:";
        int c;
        std::cin >> c;
        if (c > 1)
            throw "Превышено максимальное число соединений";
        else
            arr[i]->count = c;
    }
    else {
        std::cout << "Введите новое количество соединений для выходной клеммы:";
        int c;
        std::cin >> c;
        if (c > 3)
            throw "Превышено максимальное число соединений";
        else
            arr[i]->count = c;
    }
}

void Scheme::add_clem(struct clem* c) {
    if (amount == MAX_SIZE)
        throw "Превышено максимальное количество клеммы";
    arr[amount] = c;
    amount++;
}
