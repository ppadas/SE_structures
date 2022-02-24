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
        lab618::CDualLinkedList<Data> list;
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
        lab618::CDualLinkedList<Data> list;
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
    SUBCASE("symmetry_lists") {
        lab618::CDualLinkedList<Data> list1;
        lab618::CDualLinkedList<Data> list2;
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
        lab618::CDualLinkedList<Data> list;
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

TEST_CASE("iter") {
    SUBCASE("symmetry") {
        lab618::CDualLinkedList<Data> list;
        Data data1(1);
        Data data2(2);
        list.pushBack(data1);
        list.pushBack(data1);
        list.pushFront(data1);
        list.pushBack(data2);
        list.pushFront(data2);
        auto left_it = list.begin();
        auto right_it = list.end();
        while (left_it.isValid()) {
            CHECK(left_it.getData().value == right_it.getData().value);
            ++left_it;
            --right_it;
        }
    }

    SUBCASE("two_lists") {
        lab618::CDualLinkedList<Data> list1;
        lab618::CDualLinkedList<Data> list2;
        int size = 100;
        for (int i = 0; i < size; ++i) {
            Data data1(i);
            Data data2(size - 1 - i);
            list1.pushBack(data1);
            list2.pushBack(data2);
        }
        auto it1 = list1.begin();
        auto it2 = list2.end();
        for (int i = 0; i < size; ++i) {
            CHECK(it1.getData().value == it2.getData().value);
            ++it1;
            --it2;
        }
        CHECK_FALSE(it1.isValid());
        CHECK_FALSE(it2.isValid());
    }

    SUBCASE("getData_and_*") {
        lab618::CDualLinkedList<Data> list;
        int size = 10;
        for (int i = 0; i < size; ++i) {
            Data data(i);
            list.pushBack(data);
        }
        for (auto it = list.begin(); it.isValid(); ++it) {
            CHECK(it.getData().value == (*it).value);
        }
    }
}

TEST_CASE("erase") {
    SUBCASE("delete_first") {
        lab618::CDualLinkedList<Data> list;
        Data data1(1);
        Data data2(1);
        list.pushBack(data1);
        list.pushBack(data2);
        lab618::CDualLinkedList<Data>::CIterator begin = list.begin();
        CHECK(begin.getData().value == data1.value);
        list.erase(begin);
        CHECK(list.popFront().value == data2.value);
        CHECK(list.getSize() == 0);
    }

    SUBCASE("by_condition") {
        lab618::CDualLinkedList<Data> list;
        int size = 100;
        for (int i = 0; i < size; ++i) {
            Data data(i);
            list.pushBack(data);
        }
        for (lab618::CDualLinkedList<Data>::CIterator it = list.begin(); it.isValid(); ++it) {
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
        lab618::CDualLinkedList<Data> list;
        int size = 100;
        for (int i = 0; i < size; ++i) {
            Data data(i);
            list.pushBack(data);
        }
        int value = 0;
        for (lab618::CDualLinkedList<Data>::CIterator it = list.begin(); it.isValid(); ++it) {
            CHECK(it.getData().value == value);
            ++value;
            if (it.getData().value % 4 == 0) {
                list.erase(it);
            }
        }
    }
}
