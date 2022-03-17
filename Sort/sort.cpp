#include "sort.h"

namespace templates
{
    void bubbleSort(void **ppArray, int length, CompareSortType pCompareFunc) {
        for (int i = 0; i < length; ++i) {
            for (int j = 0; j < length - 1; ++j) {
                if (pCompareFunc(ppArray[j], ppArray[j + 1]) > 0) {
                    void* swaper = ppArray[j];
                    ppArray[j] = ppArray[j + 1];
                    ppArray[j + 1] = swaper;
                }
            }
        }
    }
}