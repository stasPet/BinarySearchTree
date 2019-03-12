/***************************************************************************
/Binary search tree/ Copyright (c) by P.S. Gennadevich. All rights reserved.
 V 0.1                                                           30.01.2018.
****************************************************************************/

#include <iostream>

template<typename TKey, typename TValue> class BinaryTree
{
public:
    // Тип для исключения, если узел не найден
    struct NoElement {};

    // Тип для узла дерева
    struct Node
    {
        // Указатель на предка узла
        Node* parent;

        // Указатель на левого потомка узла
        Node* left;
        // Указатель на правого потомка узла
        Node* right;

        // Ключ узла
        TKey key;
        // Значение узла
        TValue value;

        /* Указатель на BinaryTree, служит для обнуления указателя
           BinaryTree::root */
        BinaryTree* pProprietor;

        /* Конструктор */
        Node(
                TKey k = 0,
                TValue v = 0,
                Node* l = nullptr,
                Node* r = nullptr,
                Node* p = nullptr,
                BinaryTree* pP = nullptr
            ) :
                key{k},
                value{v},
                left{l},
                right{r},
                parent{p},
                pProprietor{pP}
        {
            std::cout << "Node::Node"
                << std::endl;
        }
        // Деструктор
        ~Node()
        {
            std::cout << "Node::~Node key: "
                << key << std::endl;
        }

        // Рекурсивное уничтожение дерева
        void Destroy()
        {
            if (left) left->Destroy();
            if (right) right->Destroy();

            delete this;
        }

        // Рекурсивный метод поиска узла по ключу
        int Find(TKey k) const
        {
            /* Если k > key поверяем указатель на правого потомка,
               если right != nullptr рекурсивно вызываем Find для
               правого потомка, если right == nullptr бросаем
               исключение NoElement */
            if (k > key)
            {
                if (right) return right->Find(k);
                else throw NoElement();
            }
            /* Если k < key поверяем указатель на левого потомка,
               если left != nullptr рекурсивно вызываем Find для
               левого потомка, если left == nullptr бросаем
               исключение NoElement */
            else if (k < key)
            {
                if (left) return left->Find(k);
                else throw NoElement();
            }
            // Если k == key возвращаем value
            else
            {
                return value;
            }
        }
        // Рекурсивный метод вставки значения v по ключу k
        void Insert(TKey k, TValue v)
        {
            /* Если k > key поверяем указатель на правого потомка,
               если right != nullptr рекурсивно вызываем Insert для
               правого потомка, если right == nullptr создаем нового
               потомка и присваеваем его полям новые значения */
            if(k > key)
            {
                if (right) right->Insert(k, v);
                else
                    right = new Node(k, v, nullptr, nullptr,
                        this, pProprietor);
            }
            /* Если k < key поверяем указатель на левого потомка,
               если left != nullptr рекурсивно вызываем Insert для
               левого потомка, если left == nullptr создаем нового
               потомка и присваеваем его полям новые значения */
            else if (k < key)
            {
                if (left) left->Insert(k, v);
                else
                {
                    left = new Node(k, v, nullptr, nullptr,
                        this, pProprietor);
                }
            }
            // Если k == key присваеваем его полям новые значения
            else
            {
                key = k;
                value = v;
            }
        }
        // Рекурсивный метод удаления узла по ключу
        void Remove(TKey k)
        {
            /* Если k > key поверяем указатель на правого потомка,
               если right != nullptr рекурсивно вызываем Remove для
               правого потомка, если right == nullptr бросаем
               исключение NoElement */
            if (k > key)
            {
                if (right) return right->Remove(k);
                else throw NoElement();
            }
            /* Если k < key поверяем указатель на левого потомка,
               если left != nullptr рекурсивно вызываем Remove для
               левого потомка, если left == nullptr бросаем
               исключение NoElement */
            else if (k < key)
            {
                if (left) return left->Remove(k);
                else throw NoElement();
            }
            // Если k == key рассматривается четыре варианта
            else
            {
                // Если нет ни правого ни левого потомка
                if (!left && !right)
                {
                    if (IsRoot() )
                        pProprietor->root = nullptr;

                    else if (IsLeftHeir() )
                        parent->left = nullptr;

                    else if (IsRightHeir() )
                        parent->right = nullptr;

                }
                // Если есть левый потомок, но нет правого
                else if (left && !right)
                {
                    if (IsRoot() )
                        pProprietor->root = left;

                    else if (IsLeftHeir() )
                        parent->left = left;

                    else if (IsRightHeir() )
                        parent->right = left;

                    left->parent = parent;
                }
                // Если есть правый потомок но нет левого
                else if (!left && right)
                {
                    if (IsRoot() )
                        pProprietor->root = right;

                    else if (IsLeftHeir() )
                        parent->left = right;

                    else if (IsRightHeir() )
                        parent->right = right;

                    right->parent = parent;
                }
                // Если есть оба потомка
                else
                {
                    // Если у правого потомка нет левого узла
                    if (!right->left)
                    {
                        right->parent = parent;
                        right->left = left;
                        left->parent = right;

                        if (IsRoot() )
                            pProprietor->root = right;
                    }
                    // Если у правого потомка есть левый узел
                    else
                    {
                        Node* lastLeft = right;
                        while (lastLeft->left)
                            lastLeft = lastLeft->left;

                        lastLeft->parent->left = nullptr;

                        lastLeft->left = left;
                        lastLeft->right = right;
                        lastLeft->parent = parent;

                        if (IsRoot() )
                            pProprietor->root = lastLeft;
                    }
                }

                // Удалить текущий узел
                delete this;
            }
        }

        // Проверка является ли узел левым потомком
        bool IsLeftHeir()
        {
            return parent->left && parent->left->key == key;
        }
        // Проверка является ли узел правым потомком
        bool IsRightHeir()
        {
            return parent->right && parent->right->key == key;
        }
        // Проверка является ли узел корнем
        bool IsRoot()
        {
            return !parent;
        }
    };

    // Указатель на корневой узел дерева
    Node* root;

public:
    // Конструктор по умолчанию root = nullptr
    BinaryTree() : root{nullptr}
    {
        // ...
    }
    // Деструктор
    ~BinaryTree()
    {
        root->Destroy();
    }

    // Интерфейс Find для связи с Node
    int Find(TKey k)
    {
        if (root == nullptr)
            throw NoElement();

        return root->Find(k);
    }
    // Интерфейс Insert для связи с Node
    void Insert(TKey k, TValue v)
    {
        if(root == nullptr)
        {
            root = new Node(k, v, nullptr, nullptr,
                nullptr, this);

            return;
        }

        root->Insert(k, v);
    }
    // Интерфейс Remove для связи с Node
    void Remove(TKey k)
    {
        if(root == nullptr)
            throw NoElement();

        root->Remove(k);
    }
};