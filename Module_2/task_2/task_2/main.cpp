#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <cassert>
#include <sstream>
#include <functional>

template<class T, class Compare>
class BinaryTree {
public:
	BinaryTree(const Compare& cmp) : root_(nullptr), cmp_(cmp) {}

	BinaryTree(const BinaryTree&) = delete; // запрет копирования
	BinaryTree& operator=(const BinaryTree&) = delete; // запрет присваивания копированием
	BinaryTree(BinaryTree&& other) noexcept { // конструктор перемещения
		root_ = other.root_;
		cmp_ = std::move(other.cmp_);
		other.root_ = nullptr;
	}
	BinaryTree& operator=(BinaryTree&& other) noexcept { // оператор перемещения
		if (this != &other) {
			deleteNode(root_);
			root_ = other.root_;
			cmp_ = std::move(other.cmp_);
			other.root_ = nullptr;
		}
		return *this;
	}

	~BinaryTree();

	// Метод, добавляющий элемент с ключом key в дерево.
	void Add(const T& key);
	// Метод, проверяющий, есть ли элемент с ключом key в дереве.
	bool Has(const T& key) const;
	// Метод, производящий обход дерева в порядке "в ширину".
	// Функциональный объект visit принимает в качестве параметра ключ элемента дерева.
	void BfsOrder(std::function<void(const T&)> visit) const;

private:
	struct Node {
		Node* Left;
		Node* Right;
		T Key;
		Node(const T& key) : Left(nullptr), Right(nullptr), Key(key) {}
	};
	Node* root_;
	Compare cmp_;

	// Метод, освобождающий память, занятую узлом node и его потомками.
	void deleteNode(Node* node);
	// Метод, добавляющий элемент с ключом key в поддерево с корнем node.
	void add(Node*& node, const T& key);
	// Метод, проверяющий, есть ли элемент с ключом key в поддереве с корнем node.
	bool has(Node* node, const T& key) const;
	// Метод, производящий обход дерева в порядке "в левом поддереве - корневой узел - в правом поддереве".
	void bfsOrder(Node* node, std::function<void(const T&)> visit) const;
};


template<class T, class Compare>
BinaryTree<T, Compare>::~BinaryTree()
{
	deleteNode(root_);
}

template<class T, class Compare>
void BinaryTree<T, Compare>::deleteNode(Node* node)
{
	if (node == nullptr) {
		return;
	}

	std::stack<Node*> nodes;
	nodes.push(node);

	while (!nodes.empty()) {
		Node* current = nodes.top();
		nodes.pop();

		if (current->Left) {
			nodes.push(current->Left);
		}
		if (current->Right) {
			nodes.push(current->Right);
		}

		delete current;
	}
}


template<class T, class Compare>
void BinaryTree<T, Compare>::Add(const T& key)
{
	add(root_, key);
}

template<class T, class Compare>
void BinaryTree<T, Compare>::add(Node*& node, const T& key)
{
	if (node == nullptr) {
		node = new Node(key);
		return;
	}

	Node* current = node;
	Node* parent = nullptr;
	while (current != nullptr) {
		parent = current;
		if (cmp_(key, current->Key)) {
			current = current->Left;
		}
		else {
			current = current->Right;
		}
	}

	if (cmp_(key, parent->Key)) {
		parent->Left = new Node(key);
	}
	else {
		parent->Right = new Node(key);
	}
}

template<class T, class Compare>
bool BinaryTree<T, Compare>::Has(const T& key) const
{
	return has(root_, key);
}

template<class T, class Compare>
bool BinaryTree<T, Compare>::has(Node* node, const T& key) const
{
	std::stack<Node*> nodesStack;

	if (node != nullptr) {
		nodesStack.push(node);
	}

	while (!nodesStack.empty()) {
		Node* current = nodesStack.top();
		nodesStack.pop();

		if (current->Key == key) {
			return true;
		}

		if (cmp(key, current->Key)) {
			if (current->Left != nullptr) {
				nodesStack.push(current->Left);
			}
		}
		else {
			if (current->Right != nullptr) {
				nodesStack.push(current->Right);
			}
		}
	}

	return false;
}

template<class T, class Compare>
void BinaryTree<T, Compare>::BfsOrder(std::function<void(const T&)> visit) const
{
	bfsOrder(root_, visit);
}

template<class T, class Compare>
void BinaryTree<T, Compare>::bfsOrder(Node* node, std::function<void(const T&)> visit) const
{
	if (node == nullptr) {
		return;
	}

	std::queue<Node*> q;
	q.push(node);

	while (!q.empty()) {
		Node* curr = q.front();
		q.pop();
		visit(curr->Key);

		if (curr->Left != nullptr) {
			q.push(curr->Left);
		}
		if (curr->Right != nullptr) {
			q.push(curr->Right);
		}
	}
}

struct IntCmp {
	bool operator()(const int l, const int r) { return l < r; }
};


void run(std::istream& input, std::ostream& output) {
	int n = 0;
	input >> n;
	IntCmp cmp;
	BinaryTree<int, IntCmp> tree(cmp);
	
	for (size_t i = 0; i < n; i++) {
		int key;
		input >> key;
		tree.Add(key);
	}
	tree.BfsOrder([&output](const int& key) { output << key << " "; });
}

void testBfs() {
	{
		std::stringstream input;
		std::stringstream output;
		input << "10\n10\n5\n4\n7\n9\n8\n6\n3\n2\n1\n";
		run(input, output);
		assert(output.str() == "10 5 4 7 3 6 9 2 8 1 ");
	}
	{
		std::stringstream input;
		std::stringstream output;
		input << "10\n9\n8\n6\n10\n4\n3\n2\n5\n1\n7\n";
		run(input, output);
		assert(output.str() == "9 8 10 6 4 7 3 5 2 1 ");
	}
	{
		std::stringstream input;
		std::stringstream output;
		input << "10\n1\n7\n10\n9\n4\n3\n8\n5\n2\n6\n";
		run(input, output);
		assert(output.str() == "1 7 4 10 3 5 9 2 6 8 ");
	}
}

int main()
{
	//testBfs();
	run(std::cin, std::cout);
	return 0;
}