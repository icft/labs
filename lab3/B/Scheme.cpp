#include "Scheme.h"

Scheme::Scheme(int in, int ou) {
    if (in + ou > MAX_SIZE)
        throw std::exception("Превышено максимальное количество клемм");
    amount = in + ou;
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
    if (a == nullptr)
        throw std::exception("Массив пустой");
    if (size > MAX_SIZE)
        throw std::exception("Превышено максимальное количество клемм");
    for (int i = 0; i < size; i++) {
        arr[i] = a[i];
    }
    amount = size;
}

std::istream& operator>>(std::istream& s, Scheme& scheme) {
    for (int i = 0; i < scheme.amount; i++) {
        struct clem d;
        s >> d.number >> d.type >> d.count >> d.signal;
        if (d.type == 0 && d.count > 1 || d.type == 1 && d.count > 3) {
            s.setstate(std::ios::failbit);
            return s;
        }
        if (d.signal != 'x' && d.signal != '0' && d.signal != '1') {
            s.setstate(std::ios::failbit);
            return s;
        }
        if (d.type != 0 && d.type != 1) {
            s.setstate(std::ios::failbit);
            return s;
        }
        scheme.arr[i] = d;
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

char Scheme::operator[](int num) const {
    int i = find(num);
    if (i > -1)
        return arr[i].signal;
    else
        throw std::exception("Нет клеммы с таким номером");
}

Scheme& Scheme::operator+=(const Scheme& scheme) {
    if(amount + scheme.amount > MAX_SIZE)
        throw std::overflow_error("Достигнуто максимальное количество элементов");
    for (int i = 0; i < scheme.amount; i++) {
        amount++;
        arr[amount - 1] = scheme.arr[i];
    }
}

const Scheme Scheme::operator+(const Scheme& scheme) {
    Scheme s;
    s.amount = amount + scheme.amount;
    if (s.amount > MAX_SIZE)
        throw std::exception("Количество элементов больше допустимого");
    for (int i = 0; i < amount; i++) {
        s.arr[i] = arr[i];
    }
    for (int i = 0; i < scheme.amount; i++) {
        s.arr[amount + i] = scheme.arr[i];
    }
    return s;
}

int Scheme::find(int num) const {
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
        if (count > arr[i].count) {
            throw std::exception("Новое количество больше старого");
        }
        arr[i].count = count;
    }
    else
        throw std::exception("Нет клеммы с таким номером");
}
