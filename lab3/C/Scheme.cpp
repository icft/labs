#include "Scheme.h"
#include <stdexcept>

Scheme::Scheme(int in, int out) {
    amount = in + out;
    arr = new struct clem[amount];
    for (int i = 0; i < in; i++) {
        arr[i].type = input;
        arr[i].count = 0;
        arr[i].signal = 'x';
    } for (int i = in; i < amount; i++) {
        arr[i].type = output;
        arr[i].count = 0;
        arr[i].signal = 'x';
    }
}

Scheme::Scheme(const struct clem* a, int size) {
   if (a == nullptr)
       throw std::exception("Массив пустой");
   amount = size;
   arr = new struct clem[amount];
   for (int i = 0; i < size; i++)
       arr[i] = a[i];
}

Scheme::Scheme(const Scheme& scheme) {
    amount = scheme.amount;
    if (amount == 0) {
        arr = nullptr;
    }
    else {
        arr = new struct clem[amount];
        for (int i = 0; i < amount; i++)
            arr[i] = scheme.arr[i];
    }
}

Scheme::Scheme(struct clem s) {
    amount = 1;
    arr = new struct clem[amount];
    arr[0] = s;
}

Scheme::Scheme(Scheme&& scheme) {
    amount = scheme.amount;
    arr = scheme.arr;
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
        delete[] arr;
        amount = scheme.amount;
        arr = scheme.arr;
        scheme.amount = 0;
        scheme.arr = nullptr;
    }
    return *this;
}

std::istream& operator>>(std::istream& s, Scheme& scheme) {
    struct clem* tmp = new struct clem[scheme.amount];
    for (int i = 0; i < scheme.amount; i++) {
        struct clem d;
        int a;
        s >> a >> d.count >> d.signal;
        d.type = static_cast<Type>(a);
        if (d.type == input && d.count > 1 || d.type == output && d.count > 3) {
            s.setstate(std::ios::failbit);
            delete[] tmp;
            return s;
        }
        if (d.signal != 'x' && d.signal != '0' && d.signal != '1') {
            s.setstate(std::ios::failbit);
            delete[] tmp;
            return s;
        }
        if (d.type != input && d.type != output) {
            s.setstate(std::ios::failbit);
            delete[] tmp;
            return s;
        }
        tmp[i] = d;
    }
    delete[] scheme.arr;
    scheme.arr = tmp;
    return s;
}

std::ostream& operator<<(std::ostream& s, const Scheme& scheme) {
    for (int i = 0; i < scheme.amount; i++) {
        std::string type;
        type = (scheme.arr[i].type == input) ? "входная" : "выходная";
        s << "Номер: " << i+1 << "; Тип: " << type << "; Количество соединений: " << scheme.arr[i].count << "; Тип сигнала: " << scheme.arr[i].signal << "\n";
    }
    return s;
}

void Scheme::operator()(int num, int c) {
    if (num <= amount)
        arr[num-1].signal = c;
    else
        throw std::exception("Нет клеммы с таким номером");
}

char Scheme::operator[](int num) const {
    if (num <=  amount)
        return arr[num-1].signal;
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
    delete[] arr;
    arr = ax;
    amount += scheme.amount;
}   

const Scheme operator+(const Scheme& scheme1, const Scheme& scheme2) {
    Scheme s;
    s.amount = scheme1.amount + scheme2.amount;
    s.arr = new struct clem[s.amount];
    for (int i = 0; i < scheme1.amount; i++) {
        s.arr[i] = scheme1.arr[i];
    }
    for (int i = 0; i < scheme2.amount; i++) {
        s.arr[scheme1.amount + i] = scheme2.arr[i];
    }
    return s;
}

//int Scheme::find(int num) const {
//    for (int i = 0; i < amount; i++) {
//        if (arr[i].number == num)
//            return i;
//    }
//    return -1;
//}

void Scheme::add_clem_connection(int num, int count) {
    if (num <= amount) {
        if ((arr[num-1].type == 0 && count > 1) or (arr[num-1].type == 1 && count > 3)) {
            throw std::exception("Превышено максимальное число соединений для клеммы");
        }
        arr[num-1].count = count;
    }
    else
        throw std::exception("Нет клеммы с таким номером");
}

void Scheme::reduce_clem_connection(int num, int count) {
    if (num <= amount) {
        if ((arr[num-1].type == input && count > 1) or (arr[num-1].type == output && count > 3)) {
            throw std::exception("Превышено максимальное число соединений для клеммы");
        }
        if (count > arr[num-1].count) {
            throw std::exception("Новое количество больше старого");
        }
        arr[num-1].count = count;
    }
    else
        throw std::exception("Нет клеммы с таким номером");
}
