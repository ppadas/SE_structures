#ifndef TEMPLATES_LIST_2022_02_03
#define TEMPLATES_LIST_2022_02_03

#include <iostream>

namespace lab618
{
    template<class T>
    class CSingleLinkedList {
    private:
        struct leaf {
            T data;
            leaf * pnext;
            leaf(T& _data, leaf * _pnext) : data(_data), pnext(_pnext) {}

            ~leaf() {
                data.~T();
            }
        };

    public:
        class CIterator {
        public:
            CIterator() = default;

            CIterator(leaf *p) : m_pBegin(nullptr), m_pCurrent(p) {}

            CIterator(const CIterator &src) = default;

            ~CIterator() = default;

            CIterator& operator = (const CIterator&  src) = default;

            bool operator != (const CIterator&  it) const {
                return !(m_pBegin == it.m_pBegin && m_pCurrent == it.m_pCurrent);
            }

            void operator++() {
                if (m_pBegin != nullptr) {
                    m_pCurrent = m_pBegin;
                    m_pBegin = nullptr;
                    return;
                }
                if (m_pCurrent != nullptr) {
                    m_pCurrent = m_pCurrent->pnext;
                }
            }

            T& getData() {
                return m_pCurrent->data;
            }

            T& operator*() {
                return m_pCurrent->data;
            }

            leaf* getLeaf() {
                return m_pCurrent;
            }

            void setLeaf(leaf* p) {
                m_pCurrent = p;
            }

            void setLeafPreBegin(leaf* p) {
                m_pBegin = p;
                m_pCurrent = nullptr;
            }

            bool isValid() {
                return m_pCurrent != nullptr;
            }

        private:
            //храним голову списка, если мы находимся перед началом
            leaf* m_pBegin = nullptr;
            // храним текущее положение
            leaf* m_pCurrent = nullptr;
        };

    public:

        CSingleLinkedList() {
            m_pBegin = nullptr;
            m_pEnd = nullptr;
        }

        virtual ~CSingleLinkedList() {
            while (m_pBegin != nullptr) {
                m_pBegin->data.~T();
                m_pBegin = m_pBegin->pnext;
            }
        }

        void pushBack(T& data) {
            if (m_pBegin == nullptr) {
                m_pBegin = new leaf(data, nullptr);
                m_pEnd = m_pBegin;
                return;
            }
            m_pEnd->pnext = new leaf(data, nullptr);
            m_pEnd = m_pEnd->pnext; 
        }

        void pushFront(T& data) {
            leaf* new_leaf = new leaf(data, m_pBegin);
            m_pBegin = new_leaf;
            if (m_pBegin == nullptr) {
                m_pEnd = m_pBegin;
            }
        }

        T popFront() {
            T data = m_pBegin->data;
            leaf* next = m_pBegin->pnext;
            delete(m_pBegin);
            m_pBegin = next;
            return data;
        }

        // изменяет состояние итератора. выставляет предыдущую позицию.
        void erase(CIterator& it) {
            if (it.getLeaf() == m_pBegin) {
                leaf* new_begin = m_pBegin->pnext;
                if (m_pEnd == m_pBegin) {
                    m_pEnd = nullptr;
                }
                m_pBegin->~leaf();
                it.setLeafPreBegin(new_begin);
                m_pBegin = new_begin;
            } else {
                leaf* previous = m_pBegin;
                while (previous->pnext != it.getLeaf()) {
                    previous = previous->pnext ;
                }
                previous->pnext = it.getLeaf()->pnext;
                if (it.getLeaf() == m_pEnd) {
                    m_pEnd = previous;
                }
                it.getLeaf()->~leaf();
                it.setLeaf(previous);
            }
        }

        int getSize() {
            if (m_pBegin == nullptr) {
                return 0;
            }
            int size = 1;
            leaf* current = m_pBegin;
            while (current != m_pEnd) {
                ++size;
                current = current->pnext;
            }
            return size;
        }

        void clear() {
            for(CIterator it = begin(); it.isValid(); ++it) {
                erase(it);
            }
        }

        CIterator begin() {
            return CIterator(m_pBegin);
        }

    private:
        leaf* m_pBegin, *m_pEnd;
    };



    template<class T>
    class CDualLinkedList
    {
    private:
        struct leaf
        {
            T data;
            leaf * pnext, *pprev;
            leaf(T& _data, leaf * _pprev, leaf * _pnext) : data(_data), pnext(_pnext), pprev(_pprev) {}
            ~leaf() {
                data.~T();
            }
        };
    public:
        class CIterator
        {
        public:
            CIterator() = default;

            CIterator(leaf *p) : m_pBegin(nullptr), m_pCurrent(p), m_pEnd(nullptr) {}

            CIterator(const CIterator &src) = default;

            ~CIterator() = default;

            CIterator& operator = (const CIterator&  src) = default;

            bool operator != (const CIterator&  it) const {
                return !(it.m_pBegin == m_pBegin && it.m_pCurrent == it.m_pCurrent && it.m_pEnd == it.m_pEnd);
            }

            void operator++() {
                if (m_pBegin != nullptr) {
                    m_pCurrent = m_pBegin;
                    m_pBegin = nullptr;
                    return;
                }
                if (m_pCurrent != nullptr) {
                    m_pCurrent = m_pCurrent->pnext;
                }
            }

            void operator--() {
                if (m_pEnd != nullptr) {
                    m_pCurrent = m_pEnd;
                    m_pEnd = nullptr;
                    return;
                }
                if (m_pCurrent != nullptr) {
                    m_pCurrent = m_pCurrent->pprev;
                }
            }

            T& getData() {
                return m_pCurrent->data;
            }

            T& operator* () {
                return m_pCurrent->data;
            }

            leaf* getLeaf() {
                return m_pCurrent;
            }

            // применяется в erase и eraseAndNext
            void setLeaf(leaf* p) {
                m_pCurrent = p;
            }

            // применяется в erase и eraseAndNext
            void setLeafPreBegin(leaf* p) {
                m_pCurrent = nullptr;
                m_pBegin = p;
            }

            // применяется в erase и eraseAndNext
            void setLeafPostEnd(leaf* p) {
                m_pCurrent = nullptr;
                m_pEnd = p;
            }

            bool isValid() {
                return m_pCurrent != nullptr;
            }

        private:
            //храним голову списка, если мы находимся перед началом
            leaf* m_pBegin = nullptr;
            // храним текущее положение
            leaf* m_pCurrent = nullptr;
            //храним конец списка, если мы находимся после конца
            leaf* m_pEnd = nullptr;
        };

    public:

        CDualLinkedList() {
        };

        virtual ~CDualLinkedList() {
        };

        void pushBack(T& data) {
            if (m_pEnd == nullptr) {
                m_pEnd = new leaf(data, nullptr, nullptr);
                m_pBegin = m_pEnd;
                return;
            }
            m_pEnd->pnext = new leaf(data, m_pEnd, nullptr);
            m_pEnd = m_pEnd->pnext; 
        }

        T popBack() {
            T data = m_pEnd->data;
            leaf* prev = m_pEnd->pprev;
            delete(m_pEnd);
            m_pEnd = prev;
            return data;
        }

        void pushFront(T& data) {
            if (m_pBegin == nullptr) {
                m_pBegin = new leaf(data, nullptr, nullptr);
                m_pEnd = m_pBegin;
                return;
            }
            m_pBegin->pprev = new leaf(data, nullptr, m_pBegin);
            m_pBegin = m_pBegin->pprev; 
        }

        T popFront() {
            T data = m_pBegin->data;
            leaf* next = m_pBegin->pnext;
            delete(m_pBegin);
            m_pBegin = next;
            return data;
        }

        // изменяет состояние итератора. выставляет предыдущую позицию.
        void erase(CIterator& it) {
            if (it.getLeaf() == m_pBegin) {
                leaf* new_begin = m_pBegin->pnext;
                if (m_pEnd == m_pBegin) {
                    m_pEnd = nullptr;
                }
                m_pBegin->~leaf();
                it.setLeafPreBegin(new_begin);
                m_pBegin = new_begin;
            } else {
                leaf* previous = it.getLeaf()->pprev;
                previous->pnext = it.getLeaf()->pnext;
                if (it.getLeaf() != m_pEnd) {
                    it.getLeaf()->pnext->pprev = previous;
                } else {
                    m_pEnd = previous;
                }
                it.getLeaf()->~leaf();
                it.setLeaf(previous);
            }
        }

        // изменяет состояние итератора. выставляет следующую позицию.
        void eraseAndNext(CIterator& it) {
            if(it.getLeaf() == m_pEnd) {
                leaf* new_end = m_pEnd->pprev;
                if (m_pBegin == m_pEnd) {
                    m_pBegin = nullptr;
                }
                m_pEnd->~leaf();
                it.setLeafPostEnd(new_end);
                m_pEnd = new_end;
            } else {
                leaf* next = it.getLeaf()->pnext;
                next->pprev = it.getLeaf()->pprev;
                if (it.getLeaf() != m_pBegin) {
                    it.getLeaf()->pprev->pnext = next;
                } else {
                    m_pBegin = next;
                }
                it.getLeaf()->~leaf();
                it.setLeaf(next);
            }
        }

        int getSize() {
            if (m_pBegin == nullptr) {
                return 0;
            }
            int size = 1;
            leaf* current = m_pBegin;
            while (current != m_pEnd) {
                ++size;
                current = current->pnext;
            }
            return size;
        }

        void clear() {
            for(CIterator it = begin(); it.isValid(); ++it) {
                erase(it);
            }
        }

        CIterator begin() {
            return CIterator(m_pBegin);
        }

        CIterator end() {
            return CIterator(m_pEnd);
        }

    private:
        leaf* m_pBegin = nullptr;
        leaf* m_pEnd = nullptr;
    };
};
#endif //#ifndef TEMPLATES_LIST_2022_02_03

