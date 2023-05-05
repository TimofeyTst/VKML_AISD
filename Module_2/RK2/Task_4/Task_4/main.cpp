#include <iostream>
#include <cassert>
#include <sstream>

template<class T, class Compare>
class AVLTree {
public:
    AVLTree(const Compare& cmp) : root_(nullptr), cmp_(cmp) {};
    AVLTree(const AVLTree&) = delete; // запрет копирования
    AVLTree& operator=(const AVLTree&) = delete; // запрет присваивания копированием

    ~AVLTree() { delete_tree(root_); }

    void insert(const T& key) {
        root_ = insert_node(root_, key);
    };

    void remove(const T& key) { root_ = remove_node(root_, key); };

    bool exists(const T& key) {
        Node* current = root_;
        while (current != nullptr) {
            if (cmp_(key, current->key)) {
                current = current->left;
            }
            else if (cmp_(current->key, key)) {
                current = current->right;
            }
            else {
                return true;
            }
        }
        return false;
    };

    T* next(const T& key) {
        Node* node = root_;
        Node* res = nullptr;
        while (node != nullptr) {
            if (cmp_(key, node->key)) {
                res = node;
                node = node->left;
            }
            else {
                node = node->right;
            }
        }
        return res ? &res->key : nullptr;
    };

    T* prev(const T& key) {
        Node* node = root_;
        Node* res = nullptr;
        while (node != nullptr) {
            if (cmp_(node->key, key)) {
                res = node;
                node = node->right;
            }
            else {
                node = node->left;
            }
        }
        return res? &res->key : nullptr;
    }

private:
    struct Node {
        T key;
        Node* left;
        Node* right;
        int height;
        int children_count;

        Node(const T& k, Node* l = nullptr, Node* r = nullptr) :
            key(k), left(l), right(r), height(1), children_count(l ? 1 : 0 + r ? 1 : 0 + 1) {};
    };

    Node* root_;
    Compare cmp_;

    void delete_tree(Node* node) {
        if (node != nullptr) {
            delete_tree(node->left);
            delete_tree(node->right);
            delete node;
        }
    };

    int height(Node* node) const {
        if (node == nullptr) {
            return 0;
        }
        else {
            return node->height;
        }
    };

    int children_count(Node* node) const {
        if (node == nullptr) {
            return 0;
        }
        else {
            return node->children_count;
        }
    };

    int balance_factor(Node* node) const { return height(node->right) - height(node->left); };

    void update_height(Node* node) {
        int left_height = height(node->left);
        int right_height = height(node->right);
        node->height = (left_height > right_height ? left_height : right_height) + 1;
        node->children_count = (node->left ? node->left->children_count : 0) +
            (node->right ? node->right->children_count : 0) + 1;
    };

    Node* rotate_right(Node* node) {
        Node* new_root = node->left;
        node->left = new_root->right;
        new_root->right = node;
        update_height(node);
        update_height(new_root);
        return new_root;
    };

    Node* rotate_left(Node* node) {
        Node* new_root = node->right;
        node->right = new_root->left;
        new_root->left = node;
        update_height(node);
        update_height(new_root);
        return new_root;
    };

    Node* balance(Node* node) {
        update_height(node);
        if (balance_factor(node) == 2) {
            if (balance_factor(node->right) < 0) {
                node->right = rotate_right(node->right);
            }
            return rotate_left(node);
        }
        if (balance_factor(node) == -2) {
            if (balance_factor(node->left) > 0) {
                node->left = rotate_left(node->left);
            }
            return rotate_right(node);
        }
        return node;
    };

    Node* insert_node(Node* node, const T& key) {
        if (!node) {
            return new Node(key);
        }
        if (cmp_(key, node->key)) {
            node->left = insert_node(node->left, key);
        }
        else {
            node->right = insert_node(node->right, key);
        }

        return balance(node);
    };

    Node* find_min_node(Node* node) {
        while (node->left) {
            node = node->left;
        }
        return node;
    };

    Node* remove_min_node(Node* node) {
        if (!node->left) {
            return node->right;
        }
        node->left = remove_min_node(node->left);
        return balance(node);
    };

    Node* remove_node(Node* node, const T& key) {
        if (!node) {
            return nullptr;
        }
        if (cmp_(key, node->key)) {
            node->left = remove_node(node->left, key);
        }
        else if (cmp_(node->key, key)) {
            node->right = remove_node(node->right, key);
        }
        else {
            Node* left = node->left;
            Node* right = node->right;
            delete node;
            if (!right) {
                return left;
            }
            Node* min_node = find_min_node(right);
            min_node->right = remove_min_node(right);
            min_node->left = left;
            return balance(min_node);
        }
        return balance(node);
    };
};

struct MoreCmp {
    bool operator()(const int l, const int r) { return l < r; }
};

void run(std::istream& input, std::ostream& output) {
    MoreCmp cmp;
    AVLTree<int, MoreCmp> avltree(cmp);

	std::string operation;
	int value = 0;
	while (input >> operation >> value) {
		if (operation == "insert") {
            if (!avltree.exists(value)) {
                avltree.insert(value);
            }
		}
        else if (operation == "delete") {
            avltree.remove(value);
        }
        else if (operation == "exists") {
            output << (avltree.exists(value) ? "true" : "false") << std::endl;
		}
		else if (operation == "prev") {
            int* res = avltree.prev(value);
            if (res == nullptr) {
                output << "none" << std::endl;
            }
            else {
                output << *res << std::endl;
            }
		}
		else if (operation == "next") {
            int* res = avltree.next(value);
            if (res == nullptr) {
                output << "none" << std::endl;
            }
            else {
                output << *res << std::endl;
            }
		}
	}
}

void test() {
	{
		std::stringstream input;
		std::stringstream output;
        input << "insert 2\ninsert 5\ninsert 3\nexists 2\nexists 4\nnext 4\nprev 4\ndelete 5\nnext 4\nprev 4\n";
		run(input, output);
		assert(output.str() == "true\nfalse\n5\n3\nnone\n3\n");
	}
}

int main()
{
	//test();
	run(std::cin, std::cout);
	return 0;
}