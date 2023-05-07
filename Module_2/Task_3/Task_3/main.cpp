#include <iostream>
#include <vector>
#include <queue>
#include <cassert>
#include <sstream>
#include <functional>

template<class T, class Compare>
class BTree {
public:
	BTree(size_t t, const Compare& cmp) : root_(nullptr), t_(t), cmp_(cmp) {}
	BTree(const BTree&) = delete; // запрет копирования
	BTree& operator=(const BTree&) = delete; // запрет присваивания копированием

	~BTree() { delete root_; }

	void insert(const T& key);
	void printByLevels(std::ostream& output) const;
	bool isEmpty() const { return !root_; }

private:
	struct Node {
		bool is_leaf; // флаг, указывающий, является ли узел листом
		std::vector<T> keys; // массив ключей
		std::vector<Node*> children;  // массив дочерних узлов

		Node(bool is_leaf) : is_leaf(is_leaf) {}
		~Node() { for (auto child : children) { delete child; } }
	};
	Node* root_;
	size_t t_;
	Compare cmp_;

	bool isNodeFull(Node* node) const { return node->keys.size() == 2 * t_ - 1; }
	void insertNonFull(Node* node, const T& key);
	void splitChild(Node* node, size_t pos);
};

template<class T, class Compare>
void BTree<T, Compare>::insert(const T& key)
{
	// если корень пуст, создаем новый узел и делаем его корнем
	if (root_ == nullptr) {
		root_ = new Node(true);
	}

	// если корень заполнен, создаем новый узел и делаем его новым корнем
	if (isNodeFull(root_)) {
		Node* new_root = new Node(false);
		new_root->children.push_back(root_);
		splitChild(new_root, 0);
		root_ = new_root;
	}

	insertNonFull(root_, key);
}

template<class T, class Compare>
void BTree<T, Compare>::insertNonFull(Node* node, const T& key)
{
	if (node->is_leaf) {
		node->keys.resize(node->keys.size() + 1);
		int pos = node->keys.size() - 2;
		while (pos >= 0 && cmp_(key, node->keys[pos])) {
			node->keys[pos + 1] = node->keys[pos];
			--pos;
		}
		node->keys[pos + 1] = key;
	}
	else {
		int pos = node->keys.size() - 1;
		while (pos >= 0 && cmp_(key, node->keys[pos])) {
			--pos;
		}
		if (isNodeFull(node->children[pos + 1])) {
			splitChild(node, pos + 1);
			if (key > node->keys[pos + 1]) {
				++pos;
			}
		}
		insertNonFull(node->children[pos + 1], key);
	}
}

template<class T, class Compare>
void BTree<T, Compare>::splitChild(Node* node, size_t pos) {
	Node* child = node->children[pos];  // выбираем полный потомок
	Node* new_child = new Node(child->is_leaf);  // создаем новый узел-потомок

	new_child->keys.resize(t_ - 1);  // переносим половину ключей из child в new_child
	for (size_t j = 0; j < t_ - 1; ++j) {
		new_child->keys[j] = child->keys[j + t_];
	}

	if (!child->is_leaf) {  // если у child есть потомки, то переносим половину их указателей в new_child
		new_child->children.resize(t_);
		for (size_t j = 0; j < t_; ++j) {
			new_child->children[j] = child->children[j + t_];
		}
		child->children.resize(t_);  // уменьшаем количество указателей в child, если он не является листом
	}

	T median_key = child->keys[t_ - 1];
	child->keys.resize(t_ - 1);  // уменьшаем количество ключей в child

	node->children.resize(node->children.size() + 1);
	for (size_t j = node->children.size() - 1; j > pos + 1; --j) {  // сдвигаем указатели вправо, чтобы освободить место для нового указателя
		node->children[j] = node->children[j - 1];
	}
	node->children[pos + 1] = new_child;  // добавляем указатель на новый узел-потомок

	node->keys.resize(node->keys.size() + 1); // увеличиваем количество ключей в node
	for (size_t j = node->keys.size() - 1; j > pos; --j) {  // сдвигаем ключи вправо, чтобы освободить место для нового ключа
		node->keys[j] = node->keys[j - 1];
	}
	node->keys[pos] = median_key;  // вставляем медианный ключ в node
}


template<class T, class Compare>
void BTree<T, Compare>::printByLevels(std::ostream& output) const {
	if (isEmpty()) {
		return;
	}

	std::queue<Node*> q;
	q.push(root_);
	while (!q.empty()) {
		size_t size = q.size();
		for (size_t i = 0; i < size; ++i) {
			Node* curr = q.front();
			q.pop();
			for (const auto& key : curr->keys) {
				output << key << " ";
			}
			if (!curr->is_leaf) {
				for (const auto& child : curr->children) {
					q.push(child);
				}
			}
		}
		output << std::endl;
	}
}

struct IntCmp {
	bool operator()(const int l, const int r) { return l < r; }
};

void run(std::istream& input, std::ostream& output) {
	int t = 0;
	input >> t;
	BTree<int, std::less<int>> btree(t, std::less<int>());

	// Пропустить символы пробела и новой строки
	input >> std::ws;

	std::string line;
	std::getline(input, line);
	std::istringstream iss(line);
	while (iss.peek() != EOF) {
		int key;
		if (iss >> key) {
			btree.insert(key);
		}
	}

	btree.printByLevels(output);
}

void testBtree() {
	{
		std::stringstream input;
		std::stringstream output;
		input << "2\n0 1 2 3 4 5 6 7 8 9";
		run(input, output);
		assert(output.str() == "3 \n1 5 7 \n0 2 4 6 8 9 \n");
	}
	{
		std::stringstream input;
		std::stringstream output;
		input << "4\n0 1 2 3 4 5 6 7 8 9";
		run(input, output);
		assert(output.str() == "3 \n0 1 2 4 5 6 7 8 9 \n");
	}
	{
		std::stringstream input;
		std::stringstream output;
		input << "1\n0";
		run(input, output);
		assert(output.str() == "0 \n");
	}
	{
		std::stringstream input;
		std::stringstream output;
		input << "5\n9 8 7 6 5 4 3 2 1 0";
		run(input, output);
		assert(output.str() == "5 \n0 1 2 3 4 6 7 8 9 \n");
	}
	{
		std::stringstream input;
		std::stringstream output;
		input << "2\n9 8 7 6 5 4 3 2 1 0";
		run(input, output);
		assert(output.str() == "6 \n2 4 8 \n0 1 3 5 7 9 \n");
	}
}

int main()
{
	//testBtree();
	run(std::cin, std::cout);
	return 0;
}