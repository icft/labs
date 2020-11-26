#pragma once
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
public:
	Node<K, V>* root;
	Bst() : size(0), root(nullptr) {}
	~Bst() { freeBst(root); }
	Bst(const Node<K, V>* x) : root(x) {}
	void freeBst(Node<K, V>* x) {
		if (x) {
			freeBst(x->left);
			freeBst(x->right);
			delete x;
		}
	}
	void inorder(Node<K, V>* x) {
		if (x) {
			inorder(x->left);
			std::cout << x->key << "\n";
			inorder(x->right);
		}
	}
	Node<K, V>* search(Node<K, V>* x, K k) {
		if (!x || k == x->key)
			return x;
		if (k < x->key)
			return search(x->left, k);
		else
			return search(x->right, k);
	}
	Node<K, V>* minimum(Node<K, V>* x) {
		if (!x->left)
			return x;
		return minimum(x->left);
	}
	Node<K, V>* maximun(Node<k, V>* x) {
		if (!x->right)
			return x;
		return maximun(x->right);
	}
	Node<K, V>* next(Node<K, V>* x) {
		if (x->right)
			minimum(x->right);
		Node<K, V>* y = x->parent;
		while (y && x == y->right) {
			x = y;
			y = y->parent;
		}
		return y;
	}
	Node<K, V>* prev(Node<K, V>* x) {
		if (x->left)
			minimum(x->left);
		Node<K, V>* y = x->parent;
		while (y && x == y->left) {
			x = y;
			y = y->parent;
		}
		return y;
	}
	void insert(Node<K, V>* x, Node<K, V>* z) {
		while (x) {
			if (z->key < x->key)
				if (x->right)
					x = x->right
				else {
					z->parent = x;
					x->right = z;
					break;
				}
			else if (z->key < x->key)
				if (x->left)
					x = x->left;
				else {
					z->parent = x;
					x->left = z;
					break;
				}
		}
	}
	void remove(Node<K, V>* x, K k) {
		if (!x)
			return x;
		if (k < x->key)
			x->left = remove(x->left, k);
		else if (k > x->key)
			x->right = remove(x->right, k);
		else if (x->left && x->right) {
			x->key = minimum(x->right)->key;
			x->right = remove(x->right, k);
		} else 
			if (x->left) {
				x->left->parent = x->parent;
				x = x->left;
			} else if (x->right) {
				x->right->parent = x->parent;
				x = x->right;
			} else {
				x = nullptr;
			}
	}
};

