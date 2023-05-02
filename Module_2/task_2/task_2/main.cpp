#include <iostream>
#include <vector>

template<class T, class Compare>
class BinaryTree {
public:
	BinaryTree(const Compare& _cmp) : root(nullptr), cmp(_cmp) {}
	~BinaryTree();
	// нужно правило трёх

	void Add(const T& key);
	bool Has(const T& key) const;

	void DfsInOrder(void (*visit)(const T& key)) const;

private:
	struct Node {
		Node* Left;
		Node* Right;
		T Key;
		Node(const T& key) : Left(nullptr), Right(nullptr), Key(key) {}
	};
	Node* root;
	Compare cmp;

	void deleteNode(Node* node);
	void add(Node*& node, const T& key);
	bool has(Node* node, const T& key) const;
	void dfsInOrder(Node* node, void (*visit)(const T& key)) const;
};





template<class T, class Compare>
BinaryTree<T, Compare>::~BinaryTree()
{
	deleteNode(root);
}

template<class T, class Compare>
void BinaryTree<T, Compare>::deleteNode(Node* node)
{
	if (node == nullptr) {
		return;
	}
	deleteNode(node->Left);
	deleteNode(node->Right);
	delete node;
}

template<class T, class Compare>
void BinaryTree<T, Compare>::Add(const T& key)
{
	add(root, key);
}

template<class T, class Compare>
void BinaryTree<T, Compare>::add(Node*& node, const T& key)
{
	if (node == nullptr) {
		node = new Node(key);
		return;
	}

	if (cmp(key, node->Key)) {
		add(node->Left, key);
	}
	else {
		add(node->Right, key);
	}
}

template<class T, class Compare>
bool BinaryTree<T, Compare>::Has(const T& key) const
{
	return has(root, key);
}

template<class T, class Compare>
bool BinaryTree<T, Compare>::has(Node* node, const T& key) const
{
	if (node == nullptr) {
		return false;
	}

	if (node->Key == key) {
		return true;
	}

	if (cmp(key, node->Key)) {
		return has(node->Left, key);
	}
	return has(node->Right, key);
}

template<class T, class Compare>
void BinaryTree<T, Compare>::DfsInOrder(void (*visit)(const T& key)) const
{
	dfsInOrder(root, visit);
}

template<class T, class Compare>
void BinaryTree<T, Compare>::dfsInOrder(Node* node, void (*visit)(const T& key)) const
{
	if (node == nullptr) {
		return;
	}

	dfsInOrder(node->Left, visit);
	visit(node->Key);
	dfsInOrder(node->Right, visit);
}

struct IntCmp {
	bool operator()(const int l, const int r) { return l < r; }
};

int main()
{
	IntCmp cmp;
	BinaryTree<int, IntCmp> tree(cmp);
	tree.Add(20);
	tree.Add(10);
	tree.Add(40);
	tree.Add(6);
	tree.Add(19);
	tree.Add(30);
	tree.Add(50);
	tree.Add(60);
	tree.DfsInOrder([](const int& key) { std::cout << key << " "; });
	return 0;
}