#ifndef AVL_HEAD_H_2022_03_24
#define AVL_HEAD_H_2022_03_24

#include <algorithm>
#include "mm.h"

namespace templates
{

    template <class T, int(*Compare)(const T *pElement, const T* pElement2) >
    class CAVLTree
    {
    private:
        struct leaf
        {
            T* pData;
            leaf *pLeft;
            leaf *pRight;
            int balanceFactor;
            int height = 1;
        };

    public:
        class CException
        {
        public:
            CException()
            {
            }
        };

    public:
        CAVLTree() :  m_Memory(10, true)
        {
        }

        virtual ~CAVLTree()
        {
            clear();
        }

        bool add(T* pElement)
        {
            bool flag = true;
            m_pRoot = addStep(pElement, m_pRoot, flag);
            return flag;
        }

        bool update(T* pElement)
        {
            leaf* node = findStep(*pElement, m_pRoot);
            if (node == nullptr) {
                add(pElement);
                return false;
            } else {
                node->pData = pElement;
                return true;
            }
        }
      
        T* find(const T& pElement)
        {
            leaf* f = findStep(pElement, m_pRoot);
            return (f == nullptr) ? nullptr : f->pData;
        }

        bool remove(const T& element)
        {
            bool flag = true;
            m_pRoot = removeStep(&element, m_pRoot, flag);
            return flag;
        }

        void clear()
        {
            clear(m_pRoot);
            m_pRoot = nullptr;
        }

        //метод не является обязательным для реализации класса,
        //используется для отладки и печати дерева
        T* childLeft(const T& pElement) {
            leaf* l = findStep(pElement, m_pRoot);
            if (l == nullptr) {
                return nullptr;
            } else {
                l = l->pLeft;
            }
            return l ? l->pData : nullptr;
        }

        //метод не является обязательным для реализации класса,
        //используется для отладки и печати дерева
        T* childRight(const T& pElement) {
            leaf* l = findStep(pElement, m_pRoot);
            if (l == nullptr) {
                return nullptr;
            } else {
                l = l->pRight;
            }
            return l ? l->pData : nullptr;
        }

        //метод не является обязательным для реализации класса,
        //используется для проверки сбалансированности дерева
        bool checkBalance() {
            return checkBalanceStep(m_pRoot);
        }

    private:
        leaf* m_pRoot = nullptr;
        lab618::CMemoryManager<leaf> m_Memory;

        void clear(leaf* node) {
            if (node == nullptr) {
                return;
            }
            clear(node->pLeft);
            clear(node->pRight);
            m_Memory.deleteObject(node);
            node = nullptr;
        }

        leaf* addStep(T* pElement, leaf* node, bool& flag) {
            if (node == nullptr) {
                leaf* new_node = m_Memory.newObject();
                new_node->pData = pElement;
                new_node->pLeft = nullptr;
                new_node->pRight = nullptr;
                new_node->height = 1;
                return new_node;
            }
            int comparasion_result = Compare(pElement, node->pData);
            if(comparasion_result < 0) {
                node->pLeft = addStep(pElement, node->pLeft, flag);
            } else if (comparasion_result > 0) {
                node->pRight = addStep(pElement, node->pRight, flag);
            } else {
                flag = false;
            }
            //updateHeight(node);
            return balance(node);
        }

        void updateHeight(leaf* node) {
            int left_height = node->pLeft ? node->pLeft->height : 0;
            int right_height = node->pRight ? node->pRight->height : 0;
            node->height = std::max(left_height, right_height) + 1;
            node->balanceFactor = left_height - right_height;
        }

        leaf* findStep(const T& pElement, leaf* node) {
            if (node == nullptr) {
                return nullptr;
            }
            if(Compare(&pElement, node->pData) < 0) {
                return findStep(pElement, node->pLeft);
            }
            else if (Compare(&pElement, node->pData) > 0) {
                return findStep(pElement, node->pRight);
            }
            return node;
        }

        leaf* minLeaf(leaf* node) {
            return (node->pLeft != nullptr) ? minLeaf(node->pLeft) : node;
        }

        leaf* removeMin(leaf* node) {
            if(node->pLeft == nullptr) {
                return node->pRight;
            }
            node->pLeft = removeMin(node->pLeft);
            return balance(node);
        }

        leaf* removeStep(const T* pElement, leaf* node, bool& flag) { //remove step
            if (node == nullptr) {
                flag = false;
                return nullptr;
            }
            int comparasion_result = Compare(pElement, node->pData);
            if (comparasion_result < 0) {
                node->pLeft = removeStep(pElement, node->pLeft, flag);
            } else if (comparasion_result > 0) {
                node->pRight = removeStep(pElement, node->pRight, flag);
            } else {
                flag = true;
                if (node->pRight == nullptr) {
                    leaf* to_return = node->pLeft;
                    m_Memory.deleteObject(node);
                    node = nullptr;
                    return to_return;
                }
                leaf* left = node->pLeft;
                leaf* right = node->pRight;
                m_Memory.deleteObject(node);
                node = nullptr;
                leaf* min_leaf = minLeaf(right);
                min_leaf->pRight = removeMin(right);
                min_leaf->pLeft = left;
                return balance(min_leaf);
            }
            return balance(node);
        }

        leaf* rightSmallRotate(leaf* vertex) {
            leaf* new_head = vertex->pLeft;
            vertex->pLeft = new_head->pRight;
            new_head->pRight = vertex;
            updateHeight(vertex);
            updateHeight(new_head);
            return new_head;
        }

        leaf* leftSmallRotate(leaf* vertex) {
            leaf* new_head = vertex->pRight;
            vertex->pRight = new_head->pLeft;
            new_head->pLeft = vertex;
            updateHeight(vertex);
            updateHeight(new_head);
            return new_head;
        }

        leaf* balance(leaf* vertex) // балансировка узла p
        {
            updateHeight(vertex);
            if (vertex->balanceFactor == -2) {
                if (vertex->pRight->balanceFactor > 0) {
                    vertex->pRight = rightSmallRotate(vertex->pRight);
                }
                return leftSmallRotate(vertex);
            }

            if(vertex->balanceFactor == 2){
                if(vertex->pLeft->balanceFactor < 0) {
                    vertex->pLeft = leftSmallRotate(vertex->pLeft);
                }
                return rightSmallRotate(vertex);
            }
            return vertex;
        }

        bool checkBalanceStep(leaf* node) {
            if (node == nullptr) {
                return true;
            }
            bool part_res = checkBalanceStep(node->pLeft) && checkBalanceStep(node->pRight);
            updateHeight(node);
            return part_res && (node->balanceFactor < 2) && (node->balanceFactor > -2);
        }
    };

}; // namespace templates

#endif // #define AVL_HEAD_H_2015_03_31
