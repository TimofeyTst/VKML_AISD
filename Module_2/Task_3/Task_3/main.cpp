template<class T>
class BTree {
public:
	BTree(size_t _t) : root(nullptr), t(_t) {}
	~BTree() { delete root; }

	void Insert(const T& key);

private:
	struct Node {
		bool IsLeaf;
		std::vector<T> Keys;
		std::vector<Node*> Children;

		Node(bool isLeaf) : IsLeaf(isLeaf) {}
		~Node() { for (Node* child : Children) { delete child; } }
	};
	Node* root;
	size_t t;

	bool isNodeFull(Node* node) const { return node->Keys.size() == 2 * t - 1; }
	void insertNonFull(Node* node, const T& key);
	void splitChild(Node* node, size_t pos);
};

template<class T>
void BTree<T>::Insert(const T& key)
{
	if (root == nullptr) {
		root = new Node(true);
	}

	if (isNodeFull(root)) {
		Node* newRoot = new Node(false);
		newRoot->Children.push_back(root);
		root = newRoot;
		splitChild(root, 0);
	}

	insertNonFull(root, key);
}


template<class T>
void BTree<T>::insertNonFull(Node* node, const T& key)
{
	if (node->IsLeaf) {
		node->Keys.resize(node->Keys.size() + 1);
		size_t pos = node->Keys.size() - 2;
		while (pos >= 0 && key < node->Keys[pos]) {
			node->Keys[pos + 1] = node->Keys[pos];
			--pos;
		}
		node->Keys[pos] = key;
	}
	else {
		size_t pos = node->Keys.size() - 1;
		while (pos >= 0 && key < node->Keys[pos]) {
			--pos;
		}
		if (isNodeFull(node->Children[pos + 1])) {
			splitChild(node, pos + 1);
			if (key > node->Keys[pos + 1]) {
				++pos;
			}
			insertNonFull(node->Children[pos + 1], key);
		}
	}
}

template<class T>
void BTree<T>::splitChild(Node* node, size_t pos)
{
	node; pos;
}

int main()
{
	BTree<int> tree(3);
	tree.Insert(213);
	return 0;
}