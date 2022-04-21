#include<iostream>
#include <sort.h>

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest.h>

//все сортировки по возрастанию

struct Data {
    Data() = default;
    Data(int a, int b) : value(a), age(b) {}
    int value;
    int age;
};

int comparator(const Data* pElem1, const Data* pElem2) {
    int vaule = pElem1->value - pElem2->value;
    if (vaule != 0) {
        return pElem1->value - pElem2->value;
    } else {
        return pElem1->age - pElem2->age;
    }
}

TEST_CASE("simple_test") {
    int size = 20;
    Data** data = new Data* [size];
    for (int i = 0; i < size / 2; ++i) {
        data[i] = new Data(i, i + 1);
    }
    for (int i = 0; i < size / 2; ++i) {
        data[size / 2 + i] = new Data(i, i);
    }
    templates::bubbleSort<Data>(data, size, comparator);
    
    for (int i = 0; i < size; i += 2) {
        CHECK(data[i]->value == i/2);
        CHECK(data[i]->age == i/2);
        CHECK(data[i + 1]->value == i/2);
        CHECK(data[i]->age == (i + 1)/2);
    }

    for (int i = 0; i < size; ++i) {
        delete data[i];
    }
    delete[] data;
}

TEST_CASE("make_heap") {
    int size = 20;
    Data** data = new Data* [size];
    for (int i = 0; i < size / 2; ++i) {
        data[i] = new Data(i, i + 1);
    }
    for (int i = 0; i < size / 2; ++i) {
        data[size / 2 + i] = new Data(i, i);
    }
    templates::make_heap<Data>(data, size, comparator);
    for (int i = 0; i < size; ++i) {
        if (2 * i + 1 < size) {
            CHECK(comparator(data[i], data[2 * i + 1]) >= 0);
        }
        if (2 * i + 2 < size) {
            CHECK(comparator(data[i], data[2 * i + 2]) >= 0);
        }
    }

    for (int i = 0; i < size; ++i) {
        delete data[i];
    }
    delete[] data;
}

TEST_CASE("heapSort simple") {
    int size = 20;
    Data** data = new Data* [size];
    for (int i = 0; i < size / 2; ++i) {
        data[i] = new Data(i, i + 1);
    }
    for (int i = 0; i < size / 2; ++i) {
        data[size / 2 + i] = new Data(i, i);
    }
    templates::heapSort<Data>(data, size, comparator);
    for (int i = 0; i < size; i += 2) {
        CHECK(data[i]->value == i/2);
        CHECK(data[i]->age == i/2);
        CHECK(data[i + 1]->value == i/2);
        CHECK(data[i]->age == (i + 1)/2);
    }

    for (int i = 0; i < size; ++i) {
        delete data[i];
    }
    delete[] data;
}

TEST_CASE("heapSort already sorted") {
    int size = 20;
    Data** data = new Data* [size];
    for (int i = 0; i < size; ++i) {
        data[i] = new Data(i, 0);
    }
    templates::heapSort<Data>(data, size, comparator);
    for (int i = 0; i < size; ++i) {
        CHECK(data[i]->value == i);
    }

    for (int i = 0; i < size; ++i) {
        delete data[i];
    }
    delete[] data;
}

TEST_CASE("heapSort reverse") {
    int size = 20;
    Data** data = new Data* [size];
    for (int i = 0; i < size; ++i) {
        data[i] = new Data(size - i - 1, 0);
    }
    templates::heapSort<Data>(data, size, comparator);
    for (int i = 0; i < size; ++i) {
        CHECK(data[i]->value == i);
    }

    for (int i = 0; i < size; ++i) {
        delete data[i];
    }
    delete[] data;
}