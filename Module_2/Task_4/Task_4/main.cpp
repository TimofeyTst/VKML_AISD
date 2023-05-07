#include <iostream>
#include <stack>
#include <cassert>
#include <sstream>

template<class T, class Compare>
class AVLTree {
public:
    AVLTree(const Compare& cmp) : root_(nullptr), cmp_(cmp) {};
    AVLTree(const AVLTree&) = delete; // запрет копирования
    AVLTree& operator=(const AVLTree&) = delete; // запрет присваивания копированием

    ~AVLTree() { delete_tree(root_); }

    int insert(const T& key) { 
        int place = get_place(root_, key, 0);
        root_ = insert_node(root_, key); 
        return place;
    };

    void remove(const T& key) { root_ = remove_node(root_, key); };
    void remove_by_place(int place) {
        Node* node = find_kth_node(root_, place);
        if (node != nullptr) {
            root_ = remove_node(root_, node->key);
        }
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

    Node* find_and_remove_min_node(Node* node) {
        // Если дерево пустое, возвращаем nullptr
        if (!node) {
            return nullptr;
        }

        Node* min_node = node;
        Node* parent = nullptr;
        std::stack<Node*> path;

        // Находим минимальный узел и сохраняем путь к нему в стек
        while (min_node->left) {
            path.push(min_node);
            parent = min_node;
            min_node = min_node->left;
        }

        if (parent) {
            parent->left = min_node->right; // Удаляем связь min_node с деревом
            // Раскручиваем стек и балансируем дерево на каждом уровне
            while (!path.empty()) {
                Node* cur_node = path.top();
                path.pop();
                cur_node = balance(cur_node);
            }
        }
        // Иначе попадаем в ситуацию, когда нода является минимальной, тогда достаточно будет вернуть ее

        // Возвращаем минимальный элемент
        return min_node;
    }



    Node* remove_node(Node* node, const T& key) {
        // Если узел не найден, возвращаем nullptr
        if (!node) {
            return nullptr;
        }
        // Если ключ меньше ключа узла, рекурсивно ищем удаляемый узел в левом поддереве
        if (cmp_(key, node->key)) {
            node->left = remove_node(node->left, key);
        }
        // Если ключ больше ключа узла, рекурсивно ищем удаляемый узел в правом поддереве
        else if (cmp_(node->key, key)) {
            node->right = remove_node(node->right, key);
        }
        // Если ключ узла совпадает с ключом, который нужно удалить
        else {
            // Сохраняем указатели на левый и правый потомков узла
            Node* left = node->left;
            Node* right = node->right;
            // Освобождаем память удаляемого узла
            delete node;
            // Если у удаляемого узла нет правого потомка, возвращаем левого потомка (возможно, nullptr)
            if (!right) {
                return left;
            }
            Node* min_node = find_and_remove_min_node(right);
            // Присваиваем левому потомку удаленного узла левого потомка крайнего левого узла в правом поддереве
            min_node->left = left;
            // Присваеваем правому потомку только если они не равны
            if (min_node != right) {
                min_node->right = right;
            }
            // Возвращаем сбалансированное дерево
            return balance(min_node);
        }
        // Возвращаем сбалансированное дерево
        return balance(node);
    };

    int get_place(Node* node, const T& key, int acc) {
        if (node == nullptr) {
            return acc;
        }

        if (cmp_(key, node->key)) {
            return get_place(node->left, key, acc);
        }

        int left_children_count = (node->left == nullptr) ? 0 : node->left->children_count;
        int right_children_count = (node->right == nullptr) ? 0 : node->right->children_count;

        if (cmp_(node->key, key)) {
            return get_place(node->right, key, acc + left_children_count + 1);
        }

        return acc + left_children_count;
    }

    Node* find_kth_node(Node* node, int k) {
        if (node == nullptr) {
            return nullptr;
        }
        int left_children_count = (node->left == nullptr) ? 0 : node->left->children_count;
        if (k == left_children_count) {
            return node;
        }
        else if (k < left_children_count) {
            return find_kth_node(node->left, k);
        }
        else {
            return find_kth_node(node->right, k - left_children_count - 1);
        }
    }
};

struct LessCmp {
    bool operator()(const int l, const int r) { return l > r; }
};

void run(std::istream& input, std::ostream& output) {
    int n = 0;
    input >> n;
    LessCmp cmp;
    AVLTree<int, LessCmp> avltree(cmp);

    for (int i = 0; i < n; ++i) {
        int command = 0;
        int key = 0;
        input >> command >> key;
        switch (command) {
        case 1:
            output << avltree.insert(key) << std::endl;
            break;
        case 2:
            avltree.remove_by_place(key);
            break;
        }
    }
}

void testArmy() {
    {
        std::stringstream input;
        std::stringstream output;
        input << "5\n1 100\n1 200\n1 50\n2 1\n1 150";
        run(input, output);
        assert(output.str() == "0\n0\n2\n1\n");
    }
    {
        std::stringstream input;
        std::stringstream output;
        input << "7\n1 100\n1 200\n1 50\n2 1\n2 1\n2 0\n1 150";
        run(input, output);
        assert(output.str() == "0\n0\n2\n0\n");
    }
}

int main()
{
    //testArmy();
    run(std::cin, std::cout);
    return 0;
}