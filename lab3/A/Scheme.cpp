#include "Scheme.h"
#include <string>
#include <iostream>

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

int Scheme::find(int num) const {
	for (int i = 0; i < amount; i++)
		if (arr[i].number == num)
			return i;
	return -1;
}

void Scheme::set_new_state(int num, char c) {
	int i = find(num);
	if (i > -1)
		arr[i].signal = c;
	else
		throw std::exception("Нет клеммы с таким номером");
}

char Scheme::get_state(int num) const {
	int i = find(num);
	if (i > -1)
		return arr[i].signal;
	else
		throw std::exception("Нет клеммы с таким номером");
}


void Scheme::add_clem_connection(int num, int count) {
	int i = find(num);
	if (i > -1) {
		if ((arr[i].type == 0 && count > 1) or (arr[i].type == 1 && count > 3)) {
			throw std::exception("Превышено максимальное число соединений для клеммы");
		}
		arr[i].count = count;
	} else
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

void Scheme::add_clem(struct clem c) {
	if (amount == MAX_SIZE)
		throw std::exception("Превышено максимальное количество клемм");
	arr[amount] = c;
	amount++;
}


std::istream& Scheme::overriding_states(std::istream& s) {
	for (int i = 0; i < amount; i++) {
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
		arr[i] = d;
	}
	return s;
}

std::ostream& Scheme::print(std::ostream& s) const {
	for (int i = 0; i < amount; i++) {
		std::string type = (arr[i].type == 0) ? "входная" : "выходная";
		s << "Номер: " << arr[i].number << "; Тип: " << type << "; Количество соединений: " << arr[i].count << "; Тип сигнала: " << arr[i].signal << "\n";
	}
	return s;
}

