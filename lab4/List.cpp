#include "List.h"

template<typename T>
struct list<T>::node {
	T* value;
	node* next_node;
	node* prev_node;
	node();
	~node();
	node(node* _next_node, node* _prev_node);
	node(T value, node* _next_node, node* _prev_node);
};

template<typename T>
struct list<T>::iterator {
	list<T>::node* this_node;
	list<T>::iterator& operator++();
	list<T>::iterator& operator--();
	list<T>::iterator operator+(int num);
	bool operator==(const list<T>::iterator& itr);
	bool operator!=(const list<T>::iterator& itr);
	T& operator*();
	iterator() = default;
	iterator(node* new_node);
	~iterator() = default;
};
