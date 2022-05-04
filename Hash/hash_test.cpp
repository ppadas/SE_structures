#include<iostream>
#include<vector>
#include "hash.h"
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include <string>

struct Data {
    Data(std::string key, int value) : key(key), value(value) {}
    std::string key = "";
    int value = 0;
};

unsigned int HashFunc(const Data* pElement) {
    int value = 0;
    for (long unsigned int i = 0; i < pElement->key.size(); ++i) {
        value += pElement->key[i];
    }
    return value;
}

int Compare (const Data* pElement, const Data* pElement2) {
    return pElement->value - pElement2->value;
}

TEST_CASE("addWithSameCache") {
    lab618::CHash<Data, &HashFunc, &Compare> hash(10, 20);
    std::string str("a");
    //Data data = Data("aaa", 12);
    int size = 20;
    std::vector<Data> samples;
    for (int i = 0; i < size; ++i) {
        samples.push_back(Data("a", i));
    }
    for (int i = 0; i < size; ++i) {
        CHECK(hash.add(&samples[i]));
    }
    for (int i = 0; i < size; ++i) {
        CHECK(hash.find(samples[i]) == &samples[i]);
    }
}

TEST_CASE("doubble add") {
    lab618::CHash<Data, &HashFunc, &Compare> hash(10, 20);
    int size = 5;
    std::vector<Data> samples;
    for (int i = 0; i < size; ++i) {
        samples.push_back(Data("a", i));
    }
    for (int i = 0; i < size; ++i) {
        samples.push_back(Data("b", i));
    }
    for (int i = 0; i < size; ++i) {
        samples.push_back(Data("c", i));
    }

    for (long unsigned int i = 0; i < samples.size(); ++i) {
        CHECK(hash.add(&samples[i]));
    }
    for (long unsigned int i = 0; i < samples.size(); i += 2) {
        CHECK_FALSE(hash.add(&samples[i]));
    }
}

TEST_CASE("simple find") {
    lab618::CHash<Data, &HashFunc, &Compare> hash(10, 20);
    Data data1 = Data("a", 12);
    Data data2 = Data("b", 13);
    Data data3 = Data("c", 14);
    hash.add(&data1);
    hash.add(&data2);
    hash.add(&data3);
    CHECK(hash.find(data1) == &data1);
    CHECK(hash.find(data2) == &data2);
    CHECK(hash.find(data3) == &data3);
}

TEST_CASE("simple remove") {
    SUBCASE("the first") {
        lab618::CHash<Data, &HashFunc, &Compare> hash(10, 20);
        Data data1 = Data("a", 12);
        Data data2 = Data("a", 13);
        Data data3 = Data("a", 14);
        CHECK(hash.add(&data1));
        CHECK(hash.add(&data2));
        CHECK(hash.add(&data3));
        CHECK(hash.find(data1) == &data1);
        CHECK(hash.find(data2) == &data2);
        CHECK(hash.find(data3) == &data3);
        hash.remove(data1);
        CHECK(hash.find(data1) == nullptr);
        CHECK(hash.find(data2) == &data2);
        CHECK(hash.find(data3) == &data3);
    }

    SUBCASE("middle") {
        lab618::CHash<Data, &HashFunc, &Compare> hash(10, 20);
        Data data1 = Data("a", 12);
        Data data2 = Data("a", 13);
        Data data3 = Data("a", 14);
        hash.add(&data1);
        hash.add(&data2);
        hash.add(&data3);
        CHECK(hash.find(data1) == &data1);
        CHECK(hash.find(data2) == &data2);
        CHECK(hash.find(data3) == &data3);
        hash.remove(data2);
        CHECK(hash.find(data1) == &data1);
        CHECK(hash.find(data2) == nullptr);
        CHECK(hash.find(data3) == &data3);
    }

    SUBCASE("the last") {
        lab618::CHash<Data, &HashFunc, &Compare> hash(10, 20);
        Data data1 = Data("a", 12);
        Data data2 = Data("a", 13);
        Data data3 = Data("a", 14);
        hash.add(&data1);
        hash.add(&data2);
        hash.add(&data3);
        CHECK(hash.find(data1) == &data1);
        CHECK(hash.find(data2) == &data2);
        CHECK(hash.find(data3) == &data3);
        hash.remove(data3);
        CHECK(hash.find(data1) == &data1);
        CHECK(hash.find(data2) == &data2);
        CHECK(hash.find(data3) == nullptr);
    }
}

TEST_CASE("find after remove") {
    lab618::CHash<Data, &HashFunc, &Compare> hash(10, 20);
    int size = 5;
    std::vector<Data> samples;
    for (int i = 0; i < size; ++i) {
        samples.push_back(Data("a", i));
    }
    for (int i = 0; i < size; ++i) {
        samples.push_back(Data("b", i));
    }
    for (int i = 0; i < size; ++i) {
        samples.push_back(Data("c", i));
    }

    for (long unsigned int i = 0; i < samples.size(); ++i) {
        CHECK(hash.add(&samples[i]));
    }
    for (long unsigned int i = 0; i < samples.size(); i += 2) {
        CHECK(hash.remove(samples[i]));
    }
    for (long unsigned int i = 1; i < samples.size(); i += 2) {
        CHECK(hash.find(samples[i]));
    }
    for (long unsigned int i = 0; i < samples.size(); i += 2) {
        CHECK_FALSE(hash.find(samples[i]));
    }
}

TEST_CASE("update") {
    lab618::CHash<Data, &HashFunc, &Compare> hash(10, 20);
    int size = 5;
    std::vector<Data> samples;
    std::vector<Data> new_data;
    for (int i = 0; i < size; ++i) {
        samples.push_back(Data("a", i));
        new_data.push_back(Data("a", -i));
    }
    for (int i = 0; i < size; ++i) {
        samples.push_back(Data("b", i));
        new_data.push_back(Data("b", -i));
    }
    for (int i = 0; i < size; ++i) {
        samples.push_back(Data("c", i));
        new_data.push_back(Data("c", -i));
    }

    for (long unsigned int i = 0; i < samples.size(); i += 2) {
        CHECK(hash.add(&samples[i]));
    }
    for (long unsigned int i = 0; i < samples.size(); ++i) {
        CHECK(hash.update(&samples[i]) == (i % 2 == 0));
    }
}