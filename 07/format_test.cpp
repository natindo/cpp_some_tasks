#include <cstddef>
#include <iostream>

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

private:
};

int main()
{

    return 0;
}