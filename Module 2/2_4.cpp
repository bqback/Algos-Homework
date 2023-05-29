/*
2_4. Дано число N < 10^6 и последовательность целых чисел из [-2^31..2^31] длиной N.
Требуется построить бинарное дерево, заданное наивным порядком вставки.
Т.е., при добавлении очередного числа K в дерево с корнем root, если root→Key ≤ K, 
    то узел K добавляется в правое поддерево root; иначе в левое поддерево root.
Требования: Рекурсия запрещена. Решение должно поддерживать передачу функции сравнения снаружи.
*/
#include <iostream>
#include <queue>
#include <cassert>
#include <climits>

#define assertm(exp, msg) assert(((void)msg, exp))

struct Int64tLeqComparator {
    bool operator()(const int64_t l, const int64_t r) {
        return l <= r;
    }
};

template <typename ValueType, typename Comparator>
class BST {
    struct Node {
        Node(const ValueType& value)
            : value(value)
            , left(nullptr)
            , right(nullptr)
        {
        }
        Node* left;
        Node* right;
        ValueType value;
    };
 public:
    BST()
        : root(nullptr)
    {
    }
    ~BST()
    {
        clean_tree(root);
        root = nullptr;
    }
    void insert(Node*& parent, const ValueType value);
    void traverse_bfs();
    Node*& get_root() { return root; }
 private:
    Node* root;
    Comparator cmp;
    
    void clean_tree(Node* root);
};

template <typename ValueType, typename Comparator>
void BST<ValueType, Comparator>::insert(Node*& parent, const ValueType value) {
    if (!parent) {
        parent = new Node(value);
        return;
    } else if (cmp(parent->value, value)) {
        insert(parent->right, value);
    } else {
        insert(parent->left, value);
    }
}

template <typename ValueType, typename Comparator>
void BST<ValueType, Comparator>::traverse_bfs() {
    if (!root) 
        return;
    
    std::queue<Node*> q;
    q.push(root);
    
    while(!q.empty()) {
        Node* node = q.front();
        std::cout << node->value << " ";
        q.pop();
        if (node->left)
            q.push(node->left);
        if (node->right)
            q.push(node->right);
    }
    std::cout << std::endl;
}

template <typename ValueType, typename Comparator>
void BST<ValueType, Comparator>::clean_tree(Node* root) {
    if (!root) 
        return;
    
    std::queue<Node*> q;
    q.push(root);
    
    while(!q.empty()) {
        Node* node = q.front();
        if (node->left)
            q.push(node->left);
        if (node->right)
            q.push(node->right);
        q.pop();
        delete node;
    }
}

int main() {
    int node_amount;
    int64_t input_value;
    BST<int64_t, Int64tLeqComparator> tree;

    std::cin >> node_amount;
    assertm(node_amount < 1000000, "N must be below 10^6");
    
    for (int i = 0; i < node_amount; i++) {
        std::cin >> input_value;
        assertm((input_value >= INT_MIN) && (input_value-1 <= INT_MAX), "K must be between 2^-31 and 2^31");
        tree.insert(tree.get_root(), input_value);
    }
    
    tree.traverse_bfs();

    return 0;
}