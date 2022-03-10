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
            // ������ ������ �����
            T* pdata;
            // ����� ���������� �����
            block *pnext;
            // ������ ��������� ������
            int firstFreeIndex;
            // ����� ����������� �����
            int usedCount;
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
        {
        }

        virtual ~CMemoryManager()
        {
        }

        // �������� ����� ������ �������� �� ���������
        T* newObject()
        {
        }

        // ���������� ������� � ���������
        bool deleteObject(T* p)
        {
        }

        // ������� ������, ������� �� m_isDeleteElementsOnDestruct
        void clear()
        {
        }
    private:

        // ������� ����� ���� ������. ����������� � newObject
        block* newBlock()
        {
        }

        // ���������� ������ ����� ������. ����������� � clear
        void deleteBlock(block *p)
        {
        }

        // ������ �����
        int m_blkSize;
        // ������ ������ ������
        block* m_pBlocks;
        // ������� ����
        block *m_pCurrentBlk;
        // ������� �� �������� ��� ������������
        bool m_isDeleteElementsOnDestruct;
    };
}; // namespace lab618

#endif // #define MEMORY_MANAGER_HEAD_H_2022_02_17
