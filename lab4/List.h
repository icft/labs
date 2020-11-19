#pragma once
#include <memory>

template<typename T>
struct list_node {
	T data;
	std::unique_ptr<node> next;
};

template<typename T>
class list {
private:
	std::unique_ptr<list_node<T>> head;
	std::unique_ptr<list_node<T>> tail;
public:
	void add(std::unique_ptr<list_node<T>>);
	void pop(std::unique_ptr<list_node<T>>);
	std::unique_ptr<node<T>> find(std::string);
};

