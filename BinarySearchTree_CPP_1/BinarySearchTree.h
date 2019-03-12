/***************************************************************************
/Binary search tree/ Copyright (c) by P.S. Gennadevich. All rights reserved.
 V 0.1                                                           30.01.2018.
****************************************************************************/

#include <iostream>

template<typename TKey, typename TValue> class BinaryTree
{
public:
    // ��� ��� ����������, ���� ���� �� ������
    struct NoElement {};

    // ��� ��� ���� ������
    struct Node
    {
        // ��������� �� ������ ����
        Node* parent;

        // ��������� �� ������ ������� ����
        Node* left;
        // ��������� �� ������� ������� ����
        Node* right;

        // ���� ����
        TKey key;
        // �������� ����
        TValue value;

        /* ��������� �� BinaryTree, ������ ��� ��������� ���������
           BinaryTree::root */
        BinaryTree* pProprietor;

        /* ����������� */
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
        // ����������
        ~Node()
        {
            std::cout << "Node::~Node key: "
                << key << std::endl;
        }

        // ����������� ����������� ������
        void Destroy()
        {
            if (left) left->Destroy();
            if (right) right->Destroy();

            delete this;
        }

        // ����������� ����� ������ ���� �� �����
        int Find(TKey k) const
        {
            /* ���� k > key �������� ��������� �� ������� �������,
               ���� right != nullptr ���������� �������� Find ���
               ������� �������, ���� right == nullptr �������
               ���������� NoElement */
            if (k > key)
            {
                if (right) return right->Find(k);
                else throw NoElement();
            }
            /* ���� k < key �������� ��������� �� ������ �������,
               ���� left != nullptr ���������� �������� Find ���
               ������ �������, ���� left == nullptr �������
               ���������� NoElement */
            else if (k < key)
            {
                if (left) return left->Find(k);
                else throw NoElement();
            }
            // ���� k == key ���������� value
            else
            {
                return value;
            }
        }
        // ����������� ����� ������� �������� v �� ����� k
        void Insert(TKey k, TValue v)
        {
            /* ���� k > key �������� ��������� �� ������� �������,
               ���� right != nullptr ���������� �������� Insert ���
               ������� �������, ���� right == nullptr ������� ������
               ������� � ����������� ��� ����� ����� �������� */
            if(k > key)
            {
                if (right) right->Insert(k, v);
                else
                    right = new Node(k, v, nullptr, nullptr,
                        this, pProprietor);
            }
            /* ���� k < key �������� ��������� �� ������ �������,
               ���� left != nullptr ���������� �������� Insert ���
               ������ �������, ���� left == nullptr ������� ������
               ������� � ����������� ��� ����� ����� �������� */
            else if (k < key)
            {
                if (left) left->Insert(k, v);
                else
                {
                    left = new Node(k, v, nullptr, nullptr,
                        this, pProprietor);
                }
            }
            // ���� k == key ����������� ��� ����� ����� ��������
            else
            {
                key = k;
                value = v;
            }
        }
        // ����������� ����� �������� ���� �� �����
        void Remove(TKey k)
        {
            /* ���� k > key �������� ��������� �� ������� �������,
               ���� right != nullptr ���������� �������� Remove ���
               ������� �������, ���� right == nullptr �������
               ���������� NoElement */
            if (k > key)
            {
                if (right) return right->Remove(k);
                else throw NoElement();
            }
            /* ���� k < key �������� ��������� �� ������ �������,
               ���� left != nullptr ���������� �������� Remove ���
               ������ �������, ���� left == nullptr �������
               ���������� NoElement */
            else if (k < key)
            {
                if (left) return left->Remove(k);
                else throw NoElement();
            }
            // ���� k == key ��������������� ������ ��������
            else
            {
                // ���� ��� �� ������� �� ������ �������
                if (!left && !right)
                {
                    if (IsRoot() )
                        pProprietor->root = nullptr;

                    else if (IsLeftHeir() )
                        parent->left = nullptr;

                    else if (IsRightHeir() )
                        parent->right = nullptr;

                }
                // ���� ���� ����� �������, �� ��� �������
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
                // ���� ���� ������ ������� �� ��� ������
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
                // ���� ���� ��� �������
                else
                {
                    // ���� � ������� ������� ��� ������ ����
                    if (!right->left)
                    {
                        right->parent = parent;
                        right->left = left;
                        left->parent = right;

                        if (IsRoot() )
                            pProprietor->root = right;
                    }
                    // ���� � ������� ������� ���� ����� ����
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

                // ������� ������� ����
                delete this;
            }
        }

        // �������� �������� �� ���� ����� ��������
        bool IsLeftHeir()
        {
            return parent->left && parent->left->key == key;
        }
        // �������� �������� �� ���� ������ ��������
        bool IsRightHeir()
        {
            return parent->right && parent->right->key == key;
        }
        // �������� �������� �� ���� ������
        bool IsRoot()
        {
            return !parent;
        }
    };

    // ��������� �� �������� ���� ������
    Node* root;

public:
    // ����������� �� ��������� root = nullptr
    BinaryTree() : root{nullptr}
    {
        // ...
    }
    // ����������
    ~BinaryTree()
    {
        root->Destroy();
    }

    // ��������� Find ��� ����� � Node
    int Find(TKey k)
    {
        if (root == nullptr)
            throw NoElement();

        return root->Find(k);
    }
    // ��������� Insert ��� ����� � Node
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
    // ��������� Remove ��� ����� � Node
    void Remove(TKey k)
    {
        if(root == nullptr)
            throw NoElement();

        root->Remove(k);
    }
};