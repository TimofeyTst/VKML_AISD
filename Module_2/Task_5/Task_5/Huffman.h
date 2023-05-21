#include <map>
#include <queue>
#include <functional>
#include <string>

using namespace std;
typedef char byte;

struct HuffmanNode {
    byte value;
    int frequency;
    HuffmanNode* left;
    HuffmanNode* right;

    HuffmanNode(byte value, int frequency) {
        this->value = value;
        this->frequency = frequency;
        left = nullptr;
        right = nullptr;
    }
    bool isLeaf() const { return left == nullptr && right == nullptr; }
};


class HuffmanTree {
private:
    HuffmanNode* root;

    void deleteTree(HuffmanNode* node) {
        if (node == nullptr) {
            return;
        }
        deleteTree(node->left);
        deleteTree(node->right);
        delete node;
    }

public:
    HuffmanTree() {
        root = nullptr;
    }

    ~HuffmanTree() {
        deleteTree(root);
    }

    void buildTree(map<byte, int> frequencies) {
        priority_queue<HuffmanNode*, vector<HuffmanNode*>, function<bool(HuffmanNode*, HuffmanNode*)>> pq(
            [](HuffmanNode* a, HuffmanNode* b) {
                return a->frequency > b->frequency;
            });

        for (auto entry : frequencies) {
            pq.push(new HuffmanNode(entry.first, entry.second));
        }

        while (pq.size() > 1) {
            HuffmanNode* left = pq.top();
            pq.pop();
            HuffmanNode* right = pq.top();
            pq.pop();
            HuffmanNode* parent = new HuffmanNode('\0', left->frequency + right->frequency);
            parent->left = left;
            parent->right = right;
            pq.push(parent);
        }

        root = pq.top();
    }

    HuffmanNode* getRoot() {
        return root;
    }

    bool isLeaf(HuffmanNode* node) {
        return (node->left == nullptr && node->right == nullptr);
    }

    void getCodes(HuffmanNode* node, map<byte, string>& codes, string code) {
        if (node == nullptr) {
            return;
        }
        if (isLeaf(node)) {
            codes[node->value] = code;
            return;
        }
        getCodes(node->left, codes, code + "0");
        getCodes(node->right, codes, code + "1");
    }

    map<byte, string> getCodes() {
        map<byte, string> codes;
        getCodes(root, codes, "");
        return codes;
    }
};
