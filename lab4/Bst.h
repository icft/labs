#pragma once
#include <iostream>


template<typename K, typename V>
class Node {
public:
	K key;
	V value;
	Node* parent;
	Node* left;
	Node* right;
	Node() : parent(nullptr), left(nullptr), right(nullptr) {}
	~Node() {
		delete parent;
		delete left;
		delete right;
	}
	Node(std::pair<K, V> _pair) : key(_pair.first), value(_pair.second), parent(nullptr), left(nullptr), right(nullptr) {}
};

template<typename K, typename V>
class Bst {
private:
	Node<K, V>* Insert_help(Node<K, V>* node, std::pair<K, V> _pair) {
		if (node == nullptr) {
			node = new Node<K, V>;
			node->key = _pair.first;
			node->value = _pair.second;
			node->left = nullptr;
			node->right = nullptr;
			node->parent = nullptr;
		}
		else if (node->key < _pair.first) {
			node->right = Insert(node->right, _pair);
			node->right->parent = node;
		}
		else {
			node->left = Insert(node->left, _pair);
			node->left->parent = node;
		}
		return node;
	}
	void PrintTreeInOrder_help(Node<K, V>* node) {
		if (node == nullptr)
			return;
		PrintTreeInOrder_help(node->left);
		std::cout << node->key << "\n";
		PrintTreeInOrder_help(node->right);
	}
	Node<K, V>* Search_help(Node<K, V>* node, K key) {
		if (node == nullptr)
			return nullptr;
		else if (node->key == key)
			return node;
		else if (node->key < key)
			return Search_help(node->right, key);
		else
			return Search_help(node->left, key);
	}
	Node<K, V>* FindMin_help(Node<K, V>* node) {
		if (node == nullptr)
			return nullptr;
		else if (node->left == nullptr)
			return node;
		else
			return FindMin_help(node->left);
	}
	Node<K, V>* FindMax_help(Node<K, V>* node) {
		if (node == nullptr)
			return nullptr;
		else if (node->right == nullptr)
			return node;
		else
			return FindMax_help(node->right);
	}
	Node<K, V>* Successor_help(Node<K, V>* node) {
		if (node->right != nullptr) {
			return FindMin_help(node->right);
		}
		else {
			Node<K, V>* parentNode = node->Parent;
			Node<K, V>* currentNode = node;
			while ((parentNode != nullptr) && (currentNode == parentNode->right)) {
				currentNode = parentNode;
				parentNode = currentNode->parent;
			}
			return parentNode;
		}
	}
	Node<K, V>* Predecessor_help(Node<K, V>* node) {
		if (node->left != nullptr) {
			return FindMax_help(node->left);
		}
		else {
			Node<K, V>* parentNode = node->Parent;
			Node<K, V>* currentNode = node;
			while ((parentNode != nullptr) && (currentNode == parentNode->left)) {
				currentNode = parentNode;
				parentNode = currentNode->parent;
			}
			return parentNode;
		}

	}
	Node<K, V>* Remove_help(Node<K, V>* node, K key) {
		if (node == nullptr)
			return nullptr;
		if (node->key == key) {
			if (node->Left == nullptr && node->Right == nullptr)
				node = nullptr;
			else if (node->Left == nullptr && node->Right != nullptr) {
				node->right->parent = node->parent;
				node = node->right;
			}
			else if (node->Left != NULL && node->Right == NULL) {
				node->left->parent = node->parent;
				node = node->left;
			}
			else
			{
				int successorKey = Successor(key);
				node->Key = successorKey;
				node->Right = Remove_help(node->Right, successorKey);
			}
		}
		else if (node->Key < key)
			node->Right = Remove_help(node->Right, key);
		else
			node->Left = Remove_helpnode->Left, key);
		return node;
	}
public:
	Node<K, V>* root;
	Bst() : root(nullptr) {}
	~Bst() { freeBst(root); }
	void freeBst(Node<K, V>* x) {
		if (x) {
			freeBst(x->left);
			freeBst(x->right);
			delete x;
		}
	}
	void insert(std::pair<K, V> _pair) {
		root = Insert_help(root, _pair);
	}
	void print_inorder() {
		PrintTreeInOrder_help(root);
	}
	Node<K, V>* search(K key) {
		Node<K, V>* res = Search_help(root, key);
		return res;
	}
	Node<K, V>* minimum() {
		return FindMin_help(root);
	}
	Node<K, V>* maximum() {
		return FindMax_help(root);
	}
	Node<K, V>* successor(K key) {
		Node <K, V>* res = Search_help(root, key);
		return (res == nullptr) ? nullptr : Successor_help(res);
	}
	Node <K, V>* predecessor(K key) {
		Node <K, V>* res = Search_help(root, key);
		return (res == nullptr) ? nullptr : Predecessor_help(res);
	}
	void remove(K key) {
		root = Remove_help(root, key);
	}
};
