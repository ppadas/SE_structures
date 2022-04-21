#include <iostream>
#include <sstream>
#include <cstdlib>
#include "mm.h"
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"


struct Data{
public:
    Data() {}

    ~Data() {}

    void setStr(std::string text) {
        str = text;
        value = 10;
    }

public:
    int value; 
    std::string str;
};

TEST_CASE("newObject") {
    lab618::CMemoryManager<Data> manager(5, true);
    std::vector<Data*> data;
    int size = 50;
    manager.newObject();
    data.push_back(manager.newObject());
    for (int i = 0; i < size; ++i) {
        data.push_back(manager.newObject());
        std::stringstream ss;
        ss << i;
        data[i]->setStr(ss.str());
    }
    for (int i = 0; i < size; ++i) {
        std::stringstream ss;
        ss << i;
        CHECK(data[i]->str == ss.str());
    }
}

TEST_CASE("deleteObject") {
    lab618::CMemoryManager<Data> manager(5, true);
    std::vector<Data*> data;
    int size = 50;
    for (int i = 0; i < size; ++i) {
        data.push_back(manager.newObject());
        data[i]->value = i;
    }
    for (int i = 0; i < size; ++i) {
        if (i % 2 == 0) {
            CHECK(manager.deleteObject(data[i]));
            CHECK(data[i]->value != i);
        }
    }
    for (int i = 0; i < size; ++i) {
        if (i % 2 == 1) {
            CHECK(manager.deleteObject(data[i]));
            CHECK(data[i]->value != i);
        }
    }
}

TEST_CASE("new after delete") {
    lab618::CMemoryManager<Data> manager(5, true);
    std::vector<Data*> data;
    int size = 50;
    for (int i = 0; i < size; ++i) {
        data.push_back(manager.newObject());
        std::stringstream ss;
        ss << i;
        data[i]->setStr(ss.str());
    }
    int amount = manager.getBlocksCount();
    for (int i = 0; i < size; ++i) {
        if (i % 2 == 0) {
            CHECK(manager.deleteObject(data[i]));
            CHECK(data[i]->value != i);
        }
    }
    CHECK(amount == manager.getBlocksCount());
    for (int i = 0; i < size; ++i) {
        if (i % 2 == 0) {
            data[i] = manager.newObject();
        }
    }
    CHECK(amount == manager.getBlocksCount());
    for (int i = 0; i < size; ++i) {
        if (i % 2 == 0) {
            data[i] = manager.newObject();
        }
    }
}