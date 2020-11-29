#pragma once
#include <iterator>
#include <vector>
#include "BinaryTree.h"

template<typename K, typename V>
class Map {
private:
	int size = 0;
	Bst<K, V>* tree;
public:
	class Iterator {
	private:
		Node<K, V>* ptr;
	public:
		Iterator() : ptr(nullptr) {}
		Iterator(Node<K, V>* p) : ptr(p) {}
		Iterator(const Iterator& it) = default;
		Iterator(Iterator&& it) = default;
		Iterator& operator =(const Iterator& it) = default;
		Iterator& operator =(Iterator&& it) = default;
		Iterator& operator ++() {
			ptr = tree->successor(ptr->key);
			return *this;
		}
		Iterator& operator ++(int) {
			Iterator tmp = *this;
			ptr = tree->successor(ptr->key);
			return tmp;
		} 
		Iterator& operator --() {
			ptr = tree->successor(ptr->key);
			return *this;
		}
		Iterator& operator --(int) {
			Iterator tmp = *this;
			ptr = tree->successor(ptr->key);
			return tmp;
		}
		bool operator!=(const Iterator& other) { return ptr != other.ptr; }
		bool operator==(const Iterator& other) { return ptr == other.ptr; }
		bool operator!=(const Node<K, V>* other) { return ptr != other; }
		bool operator==(const Node<K, V>* other) { return ptr == other; }
		operator bool() const { return ptr != nullptr; }
		operator Node<K, V> *() const { return ptr; }
		Node<K, V>* operator->() const { return ptr; }
		Node<K, V>& operator*() const { return *ptr; }
 	};
	Map() : size(0), tree(nullptr) {}	
	~Map() { delete tree; }
	Map(const Map& other);
	Map(Map&& other) : size(other.size), tree(other.tree) {
		other.size = 0;
		other.tree = nullptr;
	}
	Map& operator=(const Map& other);
	Map& operator=(Map&& other) {
		if (this != other) {
			delete tree;
			tree = other.tree;
			size = other.size;
			other.size = 0;
			other.tree = nullptr;
		}
	}
	void add(std::pair<K, V> _pair) {
		tree->insert(_pair);
		size++;
	}
	void erase(K key) { tree->remove(key); }
	void print() { tree->print_inorder(); }
	bool empty() { return (size == 0) ? true : false; }
	int size() { return size; }
	Iterator begin() {
		Node<K, V>* t = tree->minimum();
		Iterator it(t);
		return it;
	}
	Iterator end() {
		Iterator it(nullptr);
		return it;
	}
	Iterator find(const K& key) {
		Node<K, V>* t = tree->search(key);
		if (t == nullptr)
			return end();
		else {
			Iterator it(t);
			return it;
		}
	}
	Iterator find(Iterator it);
};
