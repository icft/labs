#include "Scheme.h"

Scheme::Scheme(int in, int ou) {
    amount = in + ou;
    arr = new struct clem[amount];
    for (int i = 0; i < in; i++) {
        arr[i].number = i + 1;
        arr[i].type = 0;
        arr[i].count = 0;
        arr[i].signal = 'x';
    } for (int i = in; i < amount; i++) {
        arr[i].number = i + 1;
        arr[i].type = 1;
        arr[i].count = 0;
        arr[i].signal = 'x';
    }
}

Scheme::Scheme(const struct clem* a, int size) {
   for (int i = 0; i < size; i++) {
        arr[i] = a[i];
    }
    amount = size;
}

Scheme::Scheme(const Scheme& scheme) {
    amount = scheme.amount;
    arr = new struct clem[amount];
    for (int i = 0; i < amount; i++)
        arr[i] = scheme.arr[i];
}

Scheme& Scheme::operator=(const Scheme& scheme)
{
    if (this != &scheme) {
        delete[] arr;
        amount = scheme.amount;
        arr = new struct clem[amount];
        for (int i = 0; i < amount; i++)
            arr[i] = scheme.arr[i];
    }
    return *this;
}

std::istream& operator>>(std::istream& s, Scheme& scheme) {
    for (int i = 0; i < scheme.amount; i++) {
        s >> scheme.arr[i].signal;
    }
    return s;
}

std::ostream& operator<<(std::ostream& s, const Scheme& scheme) {
    for (int i = 0; i < scheme.amount; i++) {
        std::string type, state;
        type = (scheme.arr[i].type == 0) ? "входная" : "выходная";
        s << "Номер: " << scheme.arr[i].number << "; Тип: " << type << "; Количество соединений: " << scheme.arr[i].count << "; Тип сигнала: " << scheme.arr[i].signal << "\n";
    }
    return s;
}

void Scheme::operator()(int num, int c) {
    int i = find(num);
    if (i > -1)
        arr[i].signal = c;
    else
        throw std::exception("Нет клеммы с таким номером");
}

char Scheme::operator[](int num) {
    int i = find(num);
    if (i > -1)
        return arr[i].signal;
    else
        throw std::exception("Нет клеммы с таким номером");
}

void Scheme::operator+=(struct clem c) {
    struct clem* ax = new struct clem[amount];
    for (int i = 0; i < amount; i++) {
        ax[i] = arr[i];
    }
    delete[] arr;
    amount++;
    arr = new struct clem[amount];
    for (int i = 0; i < amount; i++) {
        arr[i] = ax[i];
    }
    delete[] ax;
    arr[amount] = c;
}   

int Scheme::find(int num) {
    for (int i = 0; i < amount; i++) {
        if (arr[i].number == num)
            return i;
    }
    return -1;
}

void Scheme::add_clem_connection(int num, int count) {
    int i = find(num);
    if (i > -1) {
        if ((arr[i].type == 0 && count > 1) or (arr[i].type == 1 && count > 3)) {
            throw std::exception("Превышено максимальное число соединений для клеммы");
        }
        arr[i].count = count;
    }
    else
        throw std::exception("Нет клеммы с таким номером");
}

void Scheme::reduce_clem_connection(int num, int count) {
    int i = find(num);
    if (i > -1) {
        if ((arr[i].type == 0 && count > 1) or (arr[i].type == 1 && count > 3)) {
            throw std::exception("Превышено максимальное число соединений для клеммы");
        }
        if ((arr[i].type == 0 && count > arr[i].count) or (arr[i].type == 1 && count > arr[i].count))
        {
            throw std::exception("Новое количество больше старого");
        }
        arr[i].count = count;
    }
    else
        throw std::exception("Нет клеммы с таким номером");
}