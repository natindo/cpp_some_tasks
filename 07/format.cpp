#include <cstddef>
#include <memory>

template <
    class Key,
    class T,
    class Compare = std::less<Key>,
    class Allocator = std::allocator<std::pair<const Key, T>>>
class avl {
    using valueType = std::pair<const Key, T>;

public:
public:
private:
    struct Node {
        size_t data;
        Node* left;
        Node* right;
        int height;

        Node(const valueType& value)
            : data(value)
            , left(nullptr)
            , right(nullptr)
            , height(1)
        {
        }
    };

    int height(Node* p)
    {
        return p ? p->height : 0;
    }

    int balanceFactor(Node* p)
    {
        return height(p->right) - height(p->left);
    }

    void fixHeight(Node* p)
    {
        int hr = height(p->right);
        int hl = height(p->left);
        p->height = (hl > hr ? hl : hr) + 1;
    }

    Node* rotateRight(Node* p)
    {
        Node* q = p->left;
        p->left = q->right;
        q->right = p;
        fixHeight(p);
        fixHeight(q);
        return q;
    }

    Node* rotateLeft(Node* q)
    {
        Node* p = q->right;
        q->right = p->left;
        p->left = q;
        fixHeight(p);
        fixHeight(q);
        return p;
    }

    Node* balance(Node* p)
    {
        fixHeight(p);
        if (balanceFactor(p) == 2) {
            if (balanceFactor(p->right) < 0) {
                p->right = rotateRight(p->right);
            }
            return rotateLeft(p);
        }
        if (balanceFactor(p) == -2) {
            if (balanceFactor(p->left) < 0) {
                p->left = rotateLeft(p->left);
            }
            return rotateRight(p);
        }
        return p;
    }

    Node* insert(Node* p, valueType value)
    {
        if (p == nullptr) {
            return new Node(value);
        }
        if (value < p->data) {
            p->left = insert(p->left, value);
        } else {
            p->right = insert(p->right, value);
        }
        return balance(p);
    }

    Node* findMin(Node* p)
    {
        return p->left ? findMin(p->left) : p;
    }

    Node* removeMin(Node* p)
    {
        if (p->left == nullptr) {
            return p->right;
        }
        p->left = removeMin(p->left);
        return balance(p);
    }

    Node* remove(Node* p, valueType k)
    {
        if (p == nullptr) {
            return 0;
        }
        if (k < p->data) {
            p->left = remove(p->left, k);
        } else if (k > p->data) {
            p->right = remove(p->right, k);
        } else {
            Node* q = p->left;
            Node* d = p->right;
            delete p;
            if (d == nullptr) {
                return q;
            }
            Node* min = findMin(d);
            min->right = removeMin(d);
            min->left = q;
            return balance(min);
        }
        return balance(p);
    }

private:
};

int main()
{

    return 0;
}