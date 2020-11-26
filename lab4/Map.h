#pragma once
#include <iterator>
#include "BinaryTree.h"

template<typename K, typename V>
class Map {
private:
	int size;
	Bst<K, V>* tree;
public:
	class const_iterator {
	private:
		Node<K, V>* node;
	public:
		friend class Map;
		const_iterator(Node<K, V>* _node) : node(_node) {}
		const_iterator(const const_iterator& other) : node(other.node) {}
		const_iterator& operator ++();
		const_iterator& operator ++(int);
		const_iterator& operator --();
		const_iterator& operator --(int);
		const std::pair<const K, V>& operator *() const;
		const std::pair<const K, V>* operator ->() const;
	};
	class iterator : public const_iterator {
	public:
		iterator(const const_iterator& other) : const_iterator(other) {}
		iterator& operator ++() {
			const_iterator::operator++();
			return *this;
		}
		iterator& operator ++(int) {
			auto result = *this;
			const_iterator::operator++();
			return result;
		}
		iterator& operator --() {
			const_iterator::operator--();
			return *this;
		}
		iterator& operator --(int) {
			auto result = *this;
			const_iterator::operator--();
			return result;
		}
		const std::pair<const K, V>& operator *() const;
		const std::pair<const K, V>* operator ->() const {
			return &this->operator*();
		}
	};
	Map() : size(0), tree(nullptr) {}
	~Map() { delete(tree); }
	Map(const Map& other) : size(other.size), tree(other.tree) {}
	Map& operator =(const Map& other);
	void insert(std::pair<K, V> _pair) {
		Node<K, V>* node = new Node(_pair);
		tree->insert(tree->root, node);
	}
	void erase(K key) {
		tree->remove(tree->root, k);
	}
	void print();
	bool isEmpty() { return (size == 0) ? true : false; }
	int getSize() { return size; }
	iterator begin(); 
	iterator end();
	const_iterator cbegin();
	const_iterator cend();
	iterator find(const K& key);
	const_iterator find(const K& key);

};
