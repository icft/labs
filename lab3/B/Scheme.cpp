#include "Scheme.h"

Scheme::Scheme(int in, int ou) {
    if (in + ou > MAX_SIZE)
        throw std::exception("��������� ������������ ���������� �����");
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
    if (size > MAX_SIZE)
        throw std::exception("��������� ������������ ���������� �����");
    for (int i = 0; i < size; i++) {
        arr[i] = a[i];
    }
    amount = size;
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
        type = (scheme.arr[i].type == 0) ? "�������" : "��������";
        s << "�����: " << scheme.arr[i].number << "; ���: " << type << "; ���������� ����������: " << scheme.arr[i].count << "; ��� �������: " << scheme.arr[i].signal << "\n";
    }
    return s;
}

void Scheme::operator()(int num, int c) {
    int i = find(num);
    if (i > -1)
        arr[i].signal = c;
    else
        throw std::exception("��� ������ � ����� �������");
}

char Scheme::operator[](int num) {
    int i = find(num);
    if (i > -1)
        return arr[i].signal;
    else
        throw std::exception("��� ������ � ����� �������");
}

void Scheme::operator+=(struct clem c) {
    if(amount == MAX_SIZE)
        throw std::overflow_error("���������� ������������ ���������� ���������");
    arr[amount] = c;
    amount++;
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
            throw std::exception("��������� ������������ ����� ���������� ��� ������");
        }
        arr[i].count = count;
    }
    else
        throw std::exception("��� ������ � ����� �������");
}

void Scheme::reduce_clem_connection(int num, int count) {
    int i = find(num);
    if (i > -1) {
        if ((arr[i].type == 0 && count > 1) or (arr[i].type == 1 && count > 3)) {
            throw std::exception("��������� ������������ ����� ���������� ��� ������");
        }
        if ((arr[i].type == 0 && count > arr[i].count) or (arr[i].type == 1 && count > arr[i].count))
        {
            throw std::exception("����� ���������� ������ �������");
        }
        arr[i].count = count;
    }
    else
        throw std::exception("��� ������ � ����� �������");
}