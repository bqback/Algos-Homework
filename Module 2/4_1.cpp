/*Требование для всех вариантов Задачи 4
Решение должно поддерживать передачу функции сравнения снаружи.

4_1. Солдаты. В одной военной части решили построить в одну шеренгу по росту. 
    Т.к. часть была далеко не образцовая, то солдаты часто приходили не вовремя,
        а то их и вовсе приходилось выгонять из шеренги за плохо начищенные сапоги. 
    Однако солдаты в процессе прихода и ухода должны были всегда быть выстроены по росту –
        сначала самые высокие,
        а в конце – самые низкие. 
    За расстановку солдат отвечал прапорщик, который заметил интересную особенность –
        все солдаты в части разного роста. 
    Ваша задача состоит в том, чтобы помочь прапорщику правильно расставлять солдат,
        а именно для каждого приходящего солдата указывать, перед каким солдатом в строе он должен становится. 
Требования: скорость выполнения команды - O(log n).
Формат входных данных.
    Первая строка содержит число N – количество команд (1 ≤ N ≤ 30 000).
    В каждой следующей строке содержится описание команды:
        число 1 и X если солдат приходит в строй (X – рост солдата, натуральное число до 100 000 включительно)
        и число 2 и Y если солдата, стоящим в строе на месте Y надо удалить из строя. 
    Солдаты в строе нумеруются с нуля.
Формат выходных данных.
На каждую команду 1 (добавление в строй) вы должны выводить число K
    – номер позиции, на которую должен встать этот солдат (все стоящие за ним двигаются назад).
*/
#include <iostream>
#include <cassert>
#include <climits>
#include <queue>

#define assertm(exp, msg) assert(((void)msg, exp))

template <typename ValueType>
struct IComparator {
    enum ComparatorResults { 
        PutLeft,
        EqualTo,
        PutRight
    };
    virtual int operator()(const ValueType& l, const ValueType& r) const = 0;
};

struct HeightDescComparator : public IComparator<int> {
    int operator()(const int& l, const int& r) const {
        if (l < r)
            return PutLeft;
        else if (l > r)
            return PutRight;
        else
            return EqualTo;
    }
};

template <typename ValueType, typename Comparator>
class AVLTree {
    struct Node {
        Node(const ValueType& value)
            : value(value)
            , height(1)
            , count(1)
            , left(nullptr)
            , right(nullptr)
        {
        }
        ValueType value;
        size_t height;
        size_t count;
        Node* left;
        Node* right;
    };
 public:
    AVLTree()
        : root(nullptr)
    {
    }
    ~AVLTree()
    {
        clean_tree(root);
        root = nullptr;
    }

    bool exists(const ValueType& value) const;
    size_t get_k(const ValueType& value);
    void add(const ValueType& value);
    void remove(const ValueType& value);
    void remove_kth(const size_t k);
 private:
    Node* root;
    Comparator cmp;

    void clean_tree(Node* node);

    Node* find_min(Node* node);
    Node* remove_min(Node* node);

    Node* find_kth(const size_t k, Node* node);

    Node* add_(Node* node, const ValueType& value);
    Node* remove_(Node* node, const ValueType& value);

    size_t get_height(Node* node);
    void fix_height(Node* node);
    size_t get_count(Node* node);
    void fix_count(Node* node);
    Node* rotate_left(Node* node);
    Node* rotate_right(Node* node);
    int get_balance(Node* node);
    Node* fix_balance(Node* node);
};

template <typename ValueType, typename Comparator>
bool AVLTree<ValueType, Comparator>::exists(const ValueType& value) const {
    Node* tmp = root;
    while (tmp) {
        auto comparison = cmp(tmp->value, value);
        switch (comparison) {
            case Comparator::ComparatorResults::PutRight: {
                tmp = tmp->right;
                break;
            }
            case Comparator::ComparatorResults::EqualTo: {
                return true;
            }
            case Comparator::ComparatorResults::PutLeft: {
                tmp = tmp->left;
                break;
            }
        }
    }
    return false;
}

template <typename ValueType, typename Comparator>
size_t AVLTree<ValueType, Comparator>::get_k(const ValueType& value) {
    Node* tmp = root;
    size_t k = get_count(root->left);
    while (tmp) {
        auto comparison = cmp(tmp->value, value);
        switch (comparison) {
            case Comparator::ComparatorResults::PutRight: {
                tmp = tmp->right;
                k += (get_count(tmp->right) + 1);
                break;
            }
            case Comparator::ComparatorResults::EqualTo: {
                return k;
            }
            case Comparator::ComparatorResults::PutLeft: {
                tmp = tmp->left;
                k -= (get_count(tmp->right) + 1);
                break;
            }
        }
    }
}

template <typename ValueType, typename Comparator>
void AVLTree<ValueType, Comparator>::add(const ValueType& value) {
    root = add_(root, value);
}

template <typename ValueType, typename Comparator>
void AVLTree<ValueType, Comparator>::remove(const ValueType& value) {
    root = remove_(root, value);
}

template <typename ValueType, typename Comparator>
void AVLTree<ValueType, Comparator>::clean_tree(Node* root) {
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

template <typename ValueType, typename Comparator>
typename AVLTree<ValueType, Comparator>::Node* AVLTree<ValueType, Comparator>::find_min(Node* node) {
    while (node->left)
        node = node->left;
    return node;
}

template <typename ValueType, typename Comparator>
typename AVLTree<ValueType, Comparator>::Node* AVLTree<ValueType, Comparator>::remove_min(Node* node) {
    if (!node->left)
        return node->right;
    node->left = remove_min(node->left);
    return fix_balance(node);
}

template <typename ValueType, typename Comparator>
typename AVLTree<ValueType, Comparator>::Node* AVLTree<ValueType, Comparator>::find_kth(const size_t k, Node* node) {
    if (!node)
        return nullptr;
    if (get_count(node->left) == k)
        return node;
    else if (get_count(node->left) > k)
        find_kth(k, node->left);
    else
        find_kth(k - get_count(root->left) - 1, node->right);
    
}

template <typename ValueType, typename Comparator>
void AVLTree<ValueType, Comparator>::remove_kth(const size_t k) {
    Node* kth_node = find_kth(k, root);
    if (kth_node)
        remove(kth_node->value);
}

template <typename ValueType, typename Comparator>
typename AVLTree<ValueType, Comparator>::Node* AVLTree<ValueType, Comparator>::add_(Node* node, const ValueType& value) {
    if (!node)
        return new Node(value);
    auto comparison = cmp(node->value, value);
    switch (comparison) {
        case Comparator::ComparatorResults::PutLeft: {
            node->left = add_(node->left, value);
            break;
        }
        case Comparator::ComparatorResults::EqualTo: {
            node->right = add_(node->right, value);
            break;
        }
        case Comparator::ComparatorResults::PutRight: {
            node->right = add_(node->right, value);
            break;
        }
    }
    return fix_balance(node);
}

template <typename ValueType, typename Comparator>
typename AVLTree<ValueType, Comparator>::Node* AVLTree<ValueType, Comparator>::remove_(Node* node, const ValueType& value) {
    if (!node)
        return nullptr;
    auto comparison = cmp(node->value, value);
    switch (comparison) {
        case Comparator::ComparatorResults::PutRight: {
            node->right = remove_(node->right, value);
            break;
        }
        case Comparator::ComparatorResults::EqualTo: {
            Node* left = node->left;
            Node* right = node->right;
            delete node;
            if (!right)
                return left;
            
            Node* min = find_min(right);
            min->right = remove_min(right);
            min->left = left;
            return fix_balance(min);
        }
        case Comparator::ComparatorResults::PutLeft: {
            node->left = remove_(node->left, value);
            break;
        }
    }
    return fix_balance(node);
}

template <typename ValueType, typename Comparator>
size_t AVLTree<ValueType, Comparator>::get_height(Node* node) {
    return node ? node->height : 0;
}

template <typename ValueType, typename Comparator>
void AVLTree<ValueType, Comparator>::fix_height(Node* node) {
    node->height = std::max(get_height(node->left), get_height(node->right)) + 1;
}

template <typename ValueType, typename Comparator>
size_t AVLTree<ValueType, Comparator>::get_count(Node* node) {
    return node ? node->count : 0;
}

template <typename ValueType, typename Comparator>
void AVLTree<ValueType, Comparator>::fix_count(Node* node) {
    node->count = get_count(node->left) + get_count(node->right) + 1;
}

template <typename ValueType, typename Comparator>
int AVLTree<ValueType, Comparator>::get_balance(Node* node) {
    return get_height(node->right) - get_height(node->left);
}

template <typename ValueType, typename Comparator>
typename AVLTree<ValueType, Comparator>::Node* AVLTree<ValueType, Comparator>::rotate_left(Node* node) {
    Node* tmp = node->right;
    node->right = tmp->left;
    tmp->left = node;
    fix_height(node);
    fix_count(node);
    fix_height(tmp);
    fix_count(tmp);
    return tmp;
}

template <typename ValueType, typename Comparator>
typename AVLTree<ValueType, Comparator>::Node* AVLTree<ValueType, Comparator>::rotate_right(Node* node) {
    Node* tmp = node->left;
    node->left = tmp->right;
    tmp->right = node;
    fix_height(node);
    fix_count(node);
    fix_height(tmp);
    fix_count(tmp);
    return tmp;
}

template <typename ValueType, typename Comparator>
typename AVLTree<ValueType, Comparator>::Node* AVLTree<ValueType, Comparator>::fix_balance(Node* node) {
    fix_height(node);
    fix_count(node);
    switch (get_balance(node)) {
        case 2: {
            if (get_balance(node->right) < 0)
                node->right = rotate_right(node->right);
            return rotate_left(node);
        }
        case -2: {
            if (get_balance(node->left) > 0)
                node->left = rotate_left(node->left);
            return rotate_right(node);
        }
        default:
            return node;
    }
}

int main() {
    int command_amount;
    int command = 0, argument = 0;

    AVLTree<int, HeightDescComparator> tree;

    std::cin >> command_amount;
    assertm((command_amount >= 1) && (command_amount <= 90000), "N must be between 1 and 90000");

    for (int i = 0; i < command_amount; i++) {
        std::cin >> command >> argument;
        switch (command) {
            case 1: {
                assertm((argument >= 0) && (argument <= 100000), "Height must be a natural number up to 100000");
                tree.add(argument);
                std::cout << tree.get_k(argument) << std::endl;
                break;
            }
            case 2: {
                tree.remove_kth(argument);
                break;
            }
        }
    }

    return 0;
}