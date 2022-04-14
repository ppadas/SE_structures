#include <cstring>

#ifndef MEMORY_MANAGER_HEAD_H_2022_02_17
#define MEMORY_MANAGER_HEAD_H_2022_02_17

namespace lab618
{
    template <class T>
    class CMemoryManager
    {
    private:
        struct block
        {
            block(int m_blkSize) {
                //выделяется как char
                pdata = reinterpret_cast<T*>(new char[m_blkSize * sizeof(T)]);
                for (int i = 0; i < m_blkSize - 1; ++i) {
                    *reinterpret_cast<int*>(pdata + i) = i + 1;
                }
                *reinterpret_cast<int*>(pdata + m_blkSize - 1) = -1;
            }

            // Массив данных блока
            T* pdata = nullptr;
            // Адрес следующего блока
            block *pnext = nullptr;
            // Первая свободная ячейка
            int firstFreeIndex = 0;
            // Число заполненных ячеек
            int usedCount = 0;
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
        CMemoryManager(int _default_block_size, bool isDeleteElementsOnDestruct = false) 
        : m_blkSize(_default_block_size), m_isDeleteElementsOnDestruct(isDeleteElementsOnDestruct)
        { 
        }

        virtual ~CMemoryManager()
        {
            clear();
        }

        // Получить адрес нового элемента из менеджера
        T* newObject()
        {
            block* current = m_pBlocks;
            while(current != nullptr) {
                if (current->usedCount < m_blkSize) {
                    break;
                }
                current = current->pnext;
            }
            if (current == nullptr) {
                current = newBlock();
            }
            int free_index = current->firstFreeIndex;
            current->firstFreeIndex = *reinterpret_cast<int*>(current->pdata + current->firstFreeIndex);
            ++current->usedCount;
            T* new_element = current->pdata + free_index;
            memset(reinterpret_cast<void*>(new_element), 0, sizeof(T));
            //конструктор
            ::new(reinterpret_cast<void*>(new_element)) T;
            return new_element;
            return nullptr;
        }

        // Освободить элемент в менеджере
        bool deleteObject(T* p)
        {
            if (p == nullptr) {
                throw CException();
            }
            block* current = m_pBlocks;
            while(current != nullptr) {
                if (current->pdata <= p && current->pdata + m_blkSize > p) {
                    //деструктор
                    p->~T();
                    memset(reinterpret_cast<void*>(p), 0, sizeof(T));
                    *reinterpret_cast<int*> (p) = current->firstFreeIndex;
                    current->firstFreeIndex = p - current->pdata;
                    --current->usedCount;
                    return true;
                }
                current = current->pnext;
            }
            return false;
            
        }

        // Очистка данных, зависит от m_isDeleteElementsOnDestruct
        void clear()
        {
            block* current = m_pBlocks;
            m_pBlocks = nullptr;
            while (current != nullptr)
            {
                bool* to_free = new bool[m_blkSize];
                for (int i = 0; i < m_blkSize; ++i) {
                    to_free[i] = m_isDeleteElementsOnDestruct;
                }
                int iter = current->firstFreeIndex;
                while(iter != -1) {
                    to_free[iter] = false;
                    iter = *reinterpret_cast<int*>(current->pdata + iter);
                }
                block* next_block = current->pnext;
                deleteBlock(current, to_free);
                current = next_block;
            }
        }

        int getBlocksCount() 
        {
            int i = 0;
            block* current = m_pBlocks;
            while(current != nullptr) 
            {
                ++i;
                current = current->pnext;
            }
            return i;
        }
    private:

        // Создать новый блок данных. применяется в newObject
        block* newBlock()
        {
            if (m_pBlocks == nullptr) {
                m_pBlocks = new block(m_blkSize);
                m_pCurrentBlk = m_pBlocks;
                return m_pCurrentBlk;
            }
            m_pCurrentBlk->pnext = new block(m_blkSize);
            m_pCurrentBlk = m_pCurrentBlk->pnext;
            return m_pCurrentBlk;
        }

        // Освободить память блока данных. Применяется в clear
        void deleteBlock(block *p, bool* to_free)
        {
            for (int i = 0; i < m_blkSize; ++i) {
                if (to_free[i]) {
                    p->pdata[i].~T();
                }
            }
            delete[] reinterpret_cast<char*>(p->pdata);
            delete p;
        }

        // Размер блока
        int m_blkSize = 0;
        // Начало списка блоков
        block* m_pBlocks = nullptr;
        // Текущий блок
        block *m_pCurrentBlk = nullptr;
        // Удалять ли элементы при освобождении
        bool m_isDeleteElementsOnDestruct = false;
    };
}; // namespace lab618

#endif // #define MEMORY_MANAGER_HEAD_H_2022_02_17