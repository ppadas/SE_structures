#include "sort.h"
#include <cstring>

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

    void shiftDown(void **ppArray, int length, CompareSortType pCompareFunc, int i) {
        while(2 * i + 1 < length) {
            int left = 2 * i + 1;
            int right = 2 * i + 2;
            int min = left;
            if (right < length && pCompareFunc(ppArray[right], ppArray[left]) > 0) {
                min = right;
            }
            if (pCompareFunc(ppArray[i], ppArray[min]) >= 0) {
                break;
            }
            void* swap = ppArray[i];
            ppArray[i] = ppArray[min];
            ppArray[min] = swap;
            i = min;
        }
    }

    void make_heap(void **ppArray, int length, CompareSortType pCompareFunc) {
        for (int i = length / 2; i >= 0; --i) {
            shiftDown(ppArray, length, pCompareFunc, i);
        }
    }

    void* extract_max(void **ppArray, int heap_end, CompareSortType pCompareFunc) {
        void* min = ppArray[0];
        ppArray[0] = ppArray[heap_end - 1];
        shiftDown(ppArray, heap_end, pCompareFunc, 0);
        return min;
    }

    void heapSort(void **ppArray, int length, CompareSortType pCompareFunc) {
        make_heap(ppArray, length, pCompareFunc);
        for (int i = length; i >= 1; --i) {
            ppArray[i - 1] = extract_max(ppArray, i, pCompareFunc);
        }
    }

    void merge(void** ppArray, int start, int end, CompareSortType pCompareFunc, void** buffer_array) {
        int first_iter = start;
        int second_iter = (start + end) / 2;
        int offset = start;
        while (first_iter != (start + end) / 2 && second_iter != end) {
            if (pCompareFunc(ppArray[first_iter], ppArray[second_iter]) < 0) {
                buffer_array[offset] = ppArray[first_iter];
                ++first_iter;
            } else {
                buffer_array[offset] = ppArray[second_iter];
                ++second_iter;
            }
            ++offset;
        }
        while (first_iter != (start + end) / 2) {
            buffer_array[offset] = ppArray[first_iter];
            ++first_iter;
            ++offset;
        }
        while (second_iter != end) {
            buffer_array[offset] = ppArray[second_iter];
            ++second_iter;
            ++offset;
        }
        memcpy(ppArray + start, buffer_array + start, (end - start) * sizeof(void*));
    }

    void mergeSortStep(void** ppArray, int start, int end, CompareSortType pCompareFunc, void** buffer_array) {
        if ((end - start) == 1) {
            return;
        }
        //void** left = ppArray;
        //void** right = ppArray + (end + start) / 2;
        mergeSortStep(ppArray, start,  (end + start) / 2, pCompareFunc, buffer_array); //результаты лежать в ppArray
        mergeSortStep(ppArray,  (end + start) / 2, end, pCompareFunc, buffer_array);
        merge(ppArray, start, end, pCompareFunc, buffer_array);
    }

    void mergeSort(void** ppArray, int length, CompareSortType pCompareFunc) {
        void** buffer_array = new void*[length];
        mergeSortStep(ppArray, 0, length, pCompareFunc, buffer_array);
        delete[] buffer_array;
    }
}