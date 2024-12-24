#pragma once

#include <cstddef>
#include <functional>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <utility>
#include <vector>

/**
 *  Шаблонный класс avl — упрощённая реализация AVL-дерева.
 * 
 * Методы:
 *  - empty(), size(), clear()
 *  - contains()
 *  - operator[] (вставка или доступ по ключу)
 *  - at (доступ с исключением)
 *  - insert (возвращает { итератор, bool })
 *  - erase (удаляет по ключу)
 *  - find
 *  - итераторы begin()/end(), rbegin()/rend()
 * 
 * @tparam Key      Тип ключа
 * @tparam T        Тип значения
 * @tparam Compare  Функтор сравнения (по умолчанию std::less<Key>)
 * @tparam Allocator Аллокатор (по умолчанию std::allocator<std::pair<const Key, T>>)
 */
template <
    class Key,
    class T,
    class Compare = std::less<Key>,
    class Allocator = std::allocator<std::pair<const Key, T>>>
class avl {
    using valueType = std::pair<const Key, T>;

private:
    struct Node {
        valueType data;
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

    Node* root_;
    std::size_t size_;
    Compare comp_;
    Allocator alloc_;

private:
    // Вспомогательные методы для балансировки

    // Возвращает высоту узла (или 0, если узел nullptr)

    int height(Node* p)
    {
        return p ? p->height : 0;
    }

    // Фактор балансировки (height(right) - height(left))

    int balanceFactor(Node* p)
    {
        if (!p)
            return 0;
        return height(p->right) - height(p->left);
    }

    // Обновляет поле height для узла p

    void fixHeight(Node* p)
    {
        int hr = height(p->right);
        int hl = height(p->left);
        p->height = (hl > hr ? hl : hr) + 1;
    }

    /**
    // Правый поворот вокруг узла p
     */
    Node* rotateRight(Node* p)
    {
        Node* q = p->left;
        p->left = q->right;
        q->right = p;
        fixHeight(p);
        fixHeight(q);
        return q;
    }

    /**
     * Левый поворот вокруг узла q
     */
    Node* rotateLeft(Node* q)
    {
        Node* p = q->right;
        q->right = p->left;
        p->left = q;
        fixHeight(p);
        fixHeight(q);
        return p;
    }

    /**
     * Балансирует поддерево с корнем p
     */
    Node* balance(Node* p)
    {
        fixHeight(p);
        if (balanceFactor(p) == 2) {
            if (balanceFactor(p->right) < 0)
                p->right = rotateRight(p->right);
            return rotateLeft(p);
        }
        if (balanceFactor(p) == -2) {
            if (balanceFactor(p->left) < 0)
                p->left = rotateLeft(p->left);
            return rotateRight(p);
        }
        return p;
    }

    // Вспомогательные методы для вставки / поиска / удаления

    /**
     *  Рекурсивная вставка пары (key, value) в дерево, возвращает новый корень
     */
    Node* insert(Node* p, const valueType& value)
    {
        if (!p) {
            Node* newNode = allocateNode(value);
            ++size_;
            return newNode;
        }
        if (comp_(value.first, p->data.first)) {
            p->left = insert(p->left, value);
        } else if (comp_(p->data.first, value.first)) {
            p->right = insert(p->right, value);
        } else {
            // ключ уже есть, перезаписываем значение
            p->data.second = value.second;
            return p;
        }
        return balance(p);
    }

    /**
     *  Находит минимальный элемент (самый левый узел)
     */
    Node* findMin(Node* p)
    {
        return p->left ? findMin(p->left) : p;
    }

    /**
     *  Удаляет минимальный элемент из поддерева p, возвращая новый корень
     */
    Node* removeMin(Node* p)
    {
        if (!p)
            return nullptr;
        if (!p->left) {
            Node* right = p->right;
            deallocateNode(p);
            --size_;
            return balance(right);
        }
        p->left = removeMin(p->left);
        return balance(p);
    }

    /**
     *  Удаляет узел по ключу k, возвращает новый корень
     */
    Node* eraseNode(Node* p, const Key& k)
    {
        if (!p)
            return nullptr;
        if (comp_(k, p->data.first)) {
            p->left = eraseNode(p->left, k);
        } else if (comp_(p->data.first, k)) {
            p->right = eraseNode(p->right, k);
        } else {
            // наш узел
            if (!p->left) {
                Node* right = p->right;
                deallocateNode(p);
                --size_;
                return balance(right);
            } else if (!p->right) {
                Node* left = p->left;
                deallocateNode(p);
                --size_;
                return balance(left);
            } else {
                // два потомка
                Node* minNode = findMin(p->right);
                Key newKey = minNode->data.first;
                T newVal = minNode->data.second;
                // меняем данные (через const_cast, т.к. ключ const Key)
                Key& pKeyRef = const_cast<Key&>(p->data.first);
                pKeyRef = newKey;
                p->data.second = newVal;
                p->right = removeMin(p->right);
            }
        }
        return balance(p);
    }

    /**
     *  Рекурсивная очистка поддерева
     */
    void clearNode(Node* p)
    {
        if (!p)
            return;
        clearNode(p->left);
        clearNode(p->right);
        deallocateNode(p);
    }

    /**
     *  Рекурсивный поиск узла по ключу
     */
    Node* findNode(Node* p, const Key& k) const
    {
        if (!p)
            return nullptr;
        if (comp_(k, p->data.first)) {
            return findNode(p->left, k);
        } else if (comp_(p->data.first, k)) {
            return findNode(p->right, k);
        } else {
            return p;
        }
    }

    //==================================================================
    // Аллокация / Деаллокация узлов
    //==================================================================
    Node* allocateNode(const valueType& value)
    {
        // т.к. внутри Node есть std::pair<const Key, T>, используем ::operator new
        Node* ptr = static_cast<Node*>(::operator new(sizeof(Node)));
        try {
            new (ptr) Node(value);
        } catch (...) {
            ::operator delete(ptr);
            throw std::runtime_error("Error constructing Node");
        }
        return ptr;
    }

    void deallocateNode(Node* node)
    {
        if (!node)
            return;
        node->~Node();
        ::operator delete(node);
    }

    //==================================================================
    // In-order обход для итераторов
    //==================================================================
    void inOrder(Node* p, std::vector<Node*>& vec) const
    {
        if (!p)
            return;
        inOrder(p->left, vec);
        vec.push_back(p);
        inOrder(p->right, vec);
    }

    //==================================================================
    // Вектор для итераторов
    //==================================================================
    mutable std::vector<Node*> inorderNodes_;

public:
    //==================================================================
    // Итератор
    //==================================================================
    class avl_iterator {
    private:
        const std::vector<Node*>* nodes_;
        std::size_t index_;

    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = std::pair<const Key, T>;
        using difference_type = std::ptrdiff_t;
        using pointer = value_type*;
        using reference = value_type&;

        avl_iterator(const std::vector<Node*>* nodes, std::size_t index)
            : nodes_(nodes)
            , index_(index)
        {
        }

        avl_iterator& operator++()
        {
            ++index_;
            return *this;
        }
        avl_iterator operator++(int)
        {
            avl_iterator temp = *this;
            ++(*this);
            return temp;
        }
        avl_iterator& operator--()
        {
            --index_;
            return *this;
        }
        avl_iterator operator--(int)
        {
            avl_iterator temp = *this;
            --(*this);
            return temp;
        }

        bool operator==(const avl_iterator& other) const
        {
            return (nodes_ == other.nodes_) && (index_ == other.index_);
        }
        bool operator!=(const avl_iterator& other) const
        {
            return !(*this == other);
        }

        value_type& operator*() const
        {
            return (*nodes_)[index_]->data;
        }
        value_type* operator->() const
        {
            return &((*nodes_)[index_]->data);
        }
    };

    //==================================================================
    // Конструкторы / Деструкторы
    //==================================================================
    avl()
        : root_(nullptr)
        , size_(0)
        , comp_()
        , alloc_()
    {
    }

    ~avl()
    {
        clear();
    }

    //==================================================================
    // Основные методы
    //==================================================================

    bool empty() const
    {
        return (size_ == 0);
    }

    std::size_t size() const
    {
        return size_;
    }

    void clear()
    {
        clearNode(root_);
        root_ = nullptr;
        size_ = 0;
    }

    bool contains(const Key& key) const
    {
        return (findNode(root_, key) != nullptr);
    }

    /**
     *  Доступ по ключу (создаёт, если нет)
     */
    T& operator[](const Key& key)
    {
        root_ = insert(root_, std::make_pair(key, T {}));
        Node* found = findNode(root_, key);
        return found->data.second;
    }

    /**
     *  at() - кидает исключение, если ключ не найден
     */
    T& at(const Key& key)
    {
        Node* node = findNode(root_, key);
        if (!node) {
            throw std::out_of_range("avl::at - key not found");
        }
        return node->data.second;
    }
    const T& at(const Key& key) const
    {
        Node* node = findNode(root_, key);
        if (!node) {
            throw std::out_of_range("avl::at - key not found");
        }
        return node->data.second;
    }

    /**
     *  insert(value) - возвращает {итератор, bool} 
     *        bool=true, если вставили новый ключ
     */
    std::pair<avl_iterator, bool> insert(const valueType& value)
    {
        // Проверяем, есть ли узел с таким ключом
        Node* existing = findNode(root_, value.first);
        if (existing) {
            // Ключ уже есть, возвращаем итератор на него
            return { find(value.first), false };
        }
        // Вставляем
        root_ = insert(root_, value);
        // Возвращаем итератор на новый элемент
        return { find(value.first), true };
    }

    /**
     *  erase(key) - удаляем по ключу, возвращает 1 или 0
     */
    std::size_t erase(const Key& key)
    {
        if (!contains(key)) {
            return 0;
        }
        root_ = eraseNode(root_, key);
        return 1;
    }

    /**
     *  find(key) - возвращает итератор на элемент или end()
     */
    avl_iterator find(const Key& key)
    {
        inorderNodes_.clear();
        inOrder(root_, inorderNodes_);
        for (std::size_t i = 0; i < inorderNodes_.size(); ++i) {
            if (!comp_(key, inorderNodes_[i]->data.first) && !comp_(inorderNodes_[i]->data.first, key)) {
                return avl_iterator(&inorderNodes_, i);
            }
        }
        return end();
    }
    // const-версия find
    avl_iterator find(const Key& key) const
    {
        inorderNodes_.clear();
        inOrder(root_, inorderNodes_);
        for (std::size_t i = 0; i < inorderNodes_.size(); ++i) {
            if (!comp_(key, inorderNodes_[i]->data.first) && !comp_(inorderNodes_[i]->data.first, key)) {
                return avl_iterator(&inorderNodes_, i);
            }
        }
        return end();
    }

    // Итераторы (begin, end, rbegin, rend)
    avl_iterator begin()
    {
        inorderNodes_.clear();
        inOrder(root_, inorderNodes_);
        return avl_iterator(&inorderNodes_, 0);
    }
    avl_iterator end()
    {
        inorderNodes_.clear();
        inOrder(root_, inorderNodes_);
        return avl_iterator(&inorderNodes_, inorderNodes_.size());
    }
    avl_iterator begin() const
    {
        inorderNodes_.clear();
        inOrder(root_, inorderNodes_);
        return avl_iterator(&inorderNodes_, 0);
    }
    avl_iterator end() const
    {
        inorderNodes_.clear();
        inOrder(root_, inorderNodes_);
        return avl_iterator(&inorderNodes_, inorderNodes_.size());
    }

    avl_iterator rbegin()
    {
        inorderNodes_.clear();
        inOrder(root_, inorderNodes_);
        return avl_iterator(&inorderNodes_,
            inorderNodes_.empty() ? 0 : (inorderNodes_.size() - 1));
    }
    avl_iterator rend()
    {
        inorderNodes_.clear();
        inOrder(root_, inorderNodes_);
        // rend означает "элемент до начала", используем индекс -1
        return avl_iterator(&inorderNodes_, static_cast<std::size_t>(-1));
    }
    avl_iterator rbegin() const
    {
        inorderNodes_.clear();
        inOrder(root_, inorderNodes_);
        return avl_iterator(&inorderNodes_,
            inorderNodes_.empty() ? 0 : (inorderNodes_.size() - 1));
    }
    avl_iterator rend() const
    {
        inorderNodes_.clear();
        inOrder(root_, inorderNodes_);
        return avl_iterator(&inorderNodes_, static_cast<std::size_t>(-1));
    }
};
