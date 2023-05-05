#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <cassert>
#include <sstream>

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
	int shortestPath() const;

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
int BinaryTree<T, Compare>::shortestPath() const {
	if (!root_) { // если дерево пустое
		return 0;
	}

	std::queue<Node*> q; // очередь для обхода дерева в ширину
	q.push(root_);
	int level = 0; // текущий уровень
	while (!q.empty()) {
		++level;
		int level_size = q.size(); // количество узлов на текущем уровне
		for (int i = 0; i < level_size; ++i) {
			Node* node = q.front();
			q.pop();
			if (!node->Left && !node->Right) { // если это лист
				return level;
			}
			if (node->Left) {
				q.push(node->Left);
			}
			if (node->Right) {
				q.push(node->Right);
			}
		}
	}

	return 0; // на случай, если дерево пустое
}


struct IntCmp {
	bool operator()(const int l, const int r) { return l < r; }
};


void run(std::istream& input, std::ostream& output) {
	IntCmp cmp;
	BinaryTree<int, IntCmp> tree(cmp);

	std::string line;
	std::getline(input, line);
	std::istringstream iss(line);
	while (iss.peek() != EOF) {
		int key;
		if (iss >> key) {
			tree.Add(key);
		}
	}

	output << tree.shortestPath();
}

void test() {
	{
		std::stringstream input;
		std::stringstream output;
		input << "2 1 3";
		run(input, output);
		assert(output.str() == "2");
	}
	{
		std::stringstream input;
		std::stringstream output;
		input << "3 2 1 4";
		run(input, output);
		assert(output.str() == "2");
	}
}

int main()
{
	//test();
	run(std::cin, std::cout);
	return 0;
}