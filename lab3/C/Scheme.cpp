#include "Scheme.h"

Scheme::Scheme(int in, int out) {
    amount = in + out;
    arr = new struct clem[amount];
    for (int i = 0; i < in; i++) {
        arr[i].number = i + 1;
        arr[i].type = input;
        arr[i].count = 0;
        arr[i].signal = 'x';
    } for (int i = in; i < amount; i++) {
        arr[i].number = i + 1;
        arr[i].type = output;
        arr[i].count = 0;
        arr[i].signal = 'x';
    }
}

Scheme::Scheme(const struct clem* a, int size) {
   if (a == nullptr)
       throw std::exception("Массив пустой");
   arr = new struct clem[amount];
   for (int i = 0; i < size; i++)
       arr[i] = a[i];
   amount = size;
}

Scheme::Scheme(const Scheme& scheme) {
    amount = scheme.amount;
    arr = new struct clem[amount];
    for (int i = 0; i < amount; i++)
        arr[i] = scheme.arr[i];
}

Scheme::Scheme(Scheme&& scheme) {
    amount = scheme.amount;
    arr = new struct clem[amount];
    for (int i = 0; i < amount; i++)
        arr[i] = scheme.arr[i];
    delete[] scheme.arr;
    scheme.amount = 0;
    scheme.arr = nullptr;
}

Scheme& Scheme::operator=(const Scheme& scheme) {
    if (this != &scheme) {
        struct clem* tmp;
        try {
            tmp = new struct clem[scheme.amount];
        }
        catch (const std::bad_alloc&) {
            throw "Произошла ошибка при выделении памяти\n";
        }
        delete[] arr;
        amount = scheme.amount;
        for (int i = 0; i < amount; i++)
            tmp[i] = scheme.arr[i];
        arr = tmp;
    }
    return *this;
}

Scheme& Scheme::operator=(Scheme&& scheme) {
    if(this != &scheme) {
        struct clem* tmp;
        try {
            tmp = new struct clem[scheme.amount];
        }
        catch (const std::bad_alloc&) {
            throw "Произошла ошибка при выделении памяти\n";
        }
        delete[] arr;
        amount = scheme.amount;
        for (int i = 0; i < amount; i++)
            tmp[i] = scheme.arr[i];
        arr = tmp;
        scheme.amount = 0;
        delete[] scheme.arr;
        scheme.arr = nullptr;
    }
    return *this;
}

std::istream& operator>>(std::istream& s, Scheme& scheme) {
    for (int i = 0; i < scheme.amount; i++) {
        struct clem d;
        int a;
        s >> d.number >> a >> d.count >> d.signal;
        d.type = static_cast<Type>(a);
        if (d.type == input && d.count > 1 || d.type == output && d.count > 3) {
            s.setstate(std::ios::failbit);
            return s;
        }
        if (d.signal != 'x' && d.signal != '0' && d.signal != '1') {
            s.setstate(std::ios::failbit);
            return s;
        }
        if (d.type != input && d.type != output) {
            s.setstate(std::ios::failbit);
            return s;
        }
        scheme.arr[i] = d;
    }
    return s;
}

std::ostream& operator<<(std::ostream& s, const Scheme& scheme) {
    for (int i = 0; i < scheme.amount; i++) {
        std::string type;
        type = (scheme.arr[i].type == input) ? "входная" : "выходная";
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
    struct clem* ax;
    try {
        ax = new struct clem[amount + scheme.amount];
    } catch (const std::bad_alloc&) {
        throw "Произошла ошибка при выделении памяти\n";
    }
    for (int i = 0; i < amount; i++) {
        ax[i] = arr[i];
    }
    for (int i = amount; i < scheme.amount; i++) {
        ax[i] = scheme.arr[i];
    }
    arr = ax;
    amount += scheme.amount;
}   

const Scheme Scheme::operator+(const Scheme& scheme) const {
    Scheme s;
    s.amount = amount + scheme.amount;
    try {
        s.arr = new struct clem[amount + scheme.amount];
    } catch (const std::bad_alloc&) {
        throw "Произошла ошибка при выделении памяти\n";
    }
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
        if ((arr[i].type == input && count > 1) or (arr[i].type == output && count > 3)) {
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
