#pragma once
#include "Bst.h"
#include <iostream>

template<typename K, typename V>
class Map {
private:
	int size;
	Bst<K, V>* tree;
public:
	class Iterator {
	private:
		Node<K, V>* cur;
	public:
		Iterator(Node<K, V>* n) : cur(n) {}
		Iterator(const Iterator& other) {
			cur = new Node<K, V>;
			cur->key = other.cur->key;
			cur->value = other.cur->value;
			cur->left = other.cur->left;
			cur->right = other.cur->right;
			cur->parent = other.cur->parent;
		}
		~Iterator() {
			cur = nullptr;
		}
		Node<K, V>* get_cur() { return cur; }
		Iterator& operator=(const Iterator& other) {
			if (this != it) {
				cur = new Node<K, V>;
				cur->key = other.cur->key;
				cur->value = other.cur->value;
				cur->left = other.cur->left;
				cur->right = other.cur->right;
				cur->parent = other.cur->parent;
			}
			return *this;
		}
		Iterator& operator++() {
			if (cur == nullptr) return *this;
			cur = tree->successor(cur->key);
			return *this;
		}
		Iterator operator++(int) {
			Map<K, M>::Iterator _this = *this;
			if (cur == nullptr) return _this;
			cur = tree->successor(cur->key);
			return _this;
		}
		Iterator& operator--() {
			if (cur == nullptr) return *this;
			cur = tree->predecessor(cur->key);
			return *this;
		}
		Iterator operator--(int) {
			Map<K, M>::Iterator _this = *this;
			if (cur == nullptr) return _this;
			cur = tree->predecessor(cur->key);
			return _this;
		}
		bool operator!=(const Iterator& other) { return cur != other.cur; }
		bool operator==(const Iterator& other) { return cur == other.cur; }
		bool operator!=(const Node<K, V>* other) { return cur != other; }
		bool operator==(const Node<K, V>* other) { return cur == other; }
		operator bool() const { return ptr != nullptr; }
		operator Node<K, V>* () const { return cur; }
		Node<K, V>* operator->() const { return cur; }
		Node<K, V>& operator*() const { return *cur; }
	};
	class ConstIterator {
	private:
		Node<K, V>* cur;
	public:
		ConstIterator(const ConstIterator& it) {
			cur = new Node<K, V>;
			cur->key = other.cur->key;
			cur->value = other.cur->value;
			cur->left = other.cur->left;
			cur->right = other.cur->right;
			cur->parent = other.cur->parent;
		}
		ConstIterator(const Iterator& it) {
			cur = new Node<K, V>;
			cur->key = other.cur->key;
			cur->value = other.cur->value;
			cur->left = other.cur->left;
			cur->right = other.cur->right;
			cur->parent = other.cur->parent;
		}
		ConstIterator(Node<K, M>* n) : cur(n) {}
		~ConstIterator() {
			cur = nullptr;
		}
		Node<K, M>* get_cur() const { return cur; }
		ConstIterator& operator=(const ConstIterator& other) {
			if (this != it) {
				cur = new Node<K, V>;
				cur->key = other.cur->key;
				cur->value = other.cur->value;
				cur->left = other.cur->left;
				cur->right = other.cur->right;
				cur->parent = other.cur->parent;
			}
			return *this;
		}
		ConstIterator& operator++() {
			if (cur == nullptr) return *this;
			cur = tree->successor(cur->key);
			return *this;
		}
		ConstIterator operator++(int) {
			Map<K, M>::ConstIterator _this = *this;
			if (cur == nullptr) return _this;
			cur = tree->successor(cur->key);
			return _this;
		}
		ConstIterator& operator--() {
			if (cur == nullptr) return *this;
			cur = tree->predecessor(cur->key);
			return *this;
		}
		ConstIterator operator--(int) {
			Map<K, M>::ConstIterator _this = *this;
			if (cur == nullptr) return _this;
			cur = tree->successor(cur->key);
			return _this;
		}
		bool operator!=(const ConstIterator& other) { return cur != other.cur; }
		bool operator==(const ConstIterator& other) { return cur == other.cur; }
		bool operator!=(const Node<K, V>* other) { return cur != other; }
		bool operator==(const Node<K, V>* other) { return cur == other; }
		operator bool() const { return ptr != nullptr; }
		operator Node<K, V>* () const { return cur; }
		Node<K, V>* operator->() const { return cur; }
		Node<K, V>& operator*() const { return *cur; }
	};
	Map() : size(0), tree(nullptr) {}
	~Map() { delete tree; }
	Map(const Map& other) {
		Copy(other.tree->root);
		size = other.size;
	}
	void Copy(Node<K, V>* origin) {
		if (origin == nullptr)
			return;
		std::pair<K, V> _pair;
		_pair.first = origin->key;
		_pair.second = origin->value;
		tree->insert(_pair);
		Copy(origin->left);
		Copy(origin->right);
	}
	Map(Map&& other) : size(other.size), tree(other.tree) {
		other.size = 0;
		other.tree = nullptr;
	}
	Map& operator=(const Map& other) {
		if (this != other) {
			Copy(other.tree->root);
			size = other.size;
		}
	}
	Map& operator=(Map&& other) {
		if (this != other) {
			delete tree;
			tree = other.tree;
			size = other.size;
			other.size = 0;
			other.tree = nullptr;
		}
		return *this;
	}
	bool empty() const { return (size == 0) ? true : false; }
	int get_size() const { return size; }
	void print() const { tree->print_inorder(); }
	void add(std::pair<K, V> _pair) {
		tree->insert(_pair);
		size++;
	}
	void erase(K key) {
		tree->remove(key);
		size--;
	}
	Iterator begin() { return Iterator(tree->minimum()); }
	Iterator end() { return Iterator(nullptr); }
	ConstIterator cbegin() { return ConstIterator(tree->minimum()); }
	ConstIterator cend() { return ConstIterator(nullptr); }
	Iterator find(const K& key) {
		Node<K, V>* t = tree->search(key);
		if (t == nullptr)
			return end();
		else
			return Iterator(t);
	}
	ConstIterator find(const K& key) const {
		Node<K, V>* t = tree->search(key);
		if (t == nullptr)
			return cend();
		else
			return ConstIterator(t);	
	}
};
