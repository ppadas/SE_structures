#include<iostream>
#include "list.h"
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include <vector>

class Data{
public:
    Data(int a) : value(a) {}
    ~Data() {}
    void setValue(int val) {
        value = val;
    }

public:
    int value = 0;
};

TEST_CASE("push") {
    SUBCASE("by_hand") {
        lab618::CSingleLinkedList<Data> list;
        CHECK(list.getSize() == 0);
        Data data1(3);
        Data data2(4);
        list.pushBack(data1);
        CHECK(list.getSize() == 1);
        list.pushBack(data2);
        CHECK(list.getSize() == 2);
        list.pushFront(data2);
        CHECK(list.getSize() == 3);
    }

    SUBCASE("many_values") {
        lab618::CSingleLinkedList<Data> list;
        CHECK(list.getSize() == 0);
        int current_size = 0;
        int size = 100;
        for (int i = 0; i < size; ++i) {
            Data data(i);
            list.pushBack(data);
            CHECK(list.getSize() == current_size + 1);
            list.pushFront(data);
            CHECK(list.getSize() == current_size + 2);
            current_size += 2;
        }
    }
}

TEST_CASE("push+pop") {
    SUBCASE("symmetry") {
        lab618::CSingleLinkedList<Data> list1;
        lab618::CSingleLinkedList<Data> list2;
        int size = 100;
        for (int i = 0; i < size; ++i) {
            Data data1(i);
            Data data2(size - 1 - i);
            list1.pushBack(data1);
            list2.pushFront(data2);
        }

        for (int i = 0; i < size; ++i) {
            CHECK(list1.popFront().value == list2.popFront().value);
        }
    }

    SUBCASE("mixed order") {
        lab618::CSingleLinkedList<Data> list;
        int size = 100;
        int step = 1;
        for (int i = 0; i < size; ++i) {
            Data data(i);
            list.pushBack(data);
            if (i % 4 == 0 && i != 0) {
                CHECK(list.popFront().value == step * 4 - 1);
                step++;
            }
            list.pushFront(data);
        }
    }
}

TEST_CASE("erase") {
    SUBCASE("delete_first") {
        lab618::CSingleLinkedList<Data> list;
        Data data1(1);
        Data data2(1);
        list.pushBack(data1);
        list.pushBack(data2);
        lab618::CSingleLinkedList<Data>::CIterator begin = list.begin();
        CHECK(begin.getData().value == data1.value);
        list.erase(begin);
        CHECK(list.popFront().value == data2.value);
        CHECK(list.getSize() == 0);
    }

    SUBCASE("by_condition") {
        lab618::CSingleLinkedList<Data> list;
        int size = 100;
        for (int i = 0; i < size; ++i) {
            Data data(i);
            list.pushBack(data);
        }
        for (lab618::CSingleLinkedList<Data>::CIterator it = list.begin(); it.isValid(); ++it) {
            if (it.getData().value % 4 == 0) {
                list.erase(it);
            }
        }
        for (int i = 0; i < size; ++i) {
            if (i % 4 == 0) {
                continue;
            }
            CHECK(list.popFront().value == i);
        }
    }

    SUBCASE("all") {
        lab618::CSingleLinkedList<Data> list;
        int size = 100;
        for (int i = 0; i < size; ++i) {
            Data data(i);
            list.pushBack(data);
        }
        int value = 0;
        for (lab618::CSingleLinkedList<Data>::CIterator it = list.begin(); it.isValid(); ++it) {
            CHECK(it.getData().value == value);
            ++value;
            if (it.getData().value % 4 == 0) {
                list.erase(it);
            }
        }
    }
}

TEST_CASE("clear + push") {
    lab618::CSingleLinkedList<Data> list;
    int size = 100;
    for (int i = 0; i < size; ++i) {
        Data data(i);
        list.pushBack(data);
    }
    list.clear();
    CHECK(list.getSize() == 0);
    Data data(10);
    list.pushBack(data);
    CHECK(list.getSize() == 1);
}