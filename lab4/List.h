#pragma once
#include <memory>
#include <iterator>

template<typename T>
class list {
private:
	struct node;
	node* lbegin;
	node* lend;
	int size;
public:
	struct iterator;
	typename list<T>::iterator begin();
	typename list<T>::iterator end();
	const typename list<T>::iterator cbegin();
	const typename list<T>::iterator cend();
	list();
	~list();
	list(const list&);
	list(list&&);
	list& operator= (const list&);
	list& operator= (list&&);
	bool is_empty();
	int list_size();
	T& front();
	T& back();
	void push_back(T);
	void push_front(T);
	void pop_back();
	void pop(int);
	void pop_front();
	void insert(list<T>::iterator, T);
	void clear();
};
