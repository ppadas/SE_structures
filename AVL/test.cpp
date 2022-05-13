#include<iostream>
#include<vector>
#include <algorithm>
#include "avltree.h"
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "./test_lib/doctest.h"
#include <string>
#include <random>
#include <chrono> 

struct Data {
    Data(std::string key, int value) : key(key), value(value) {}
    Data(const Data& other) = default;
    Data& operator=(const Data&) = default;
    std::string key = "";
    int value = 0;
    int inner_data = 0;
};

int Compare (const Data* pElement, const Data* pElement2) {
    return pElement->value - pElement2->value;
}

void printTree(std::vector<Data>& data, templates::CAVLTree<Data, Compare>& my_tree) {
    std::cout << "--------Tree-------\n";
    for (long unsigned int i = 0; i < data.size(); ++i) {
        std::cout << data[i].value << " : ";
        if (!my_tree.find(data[i])) {
            std::cout << "\n";
            continue;
        }
        Data* left = my_tree.childLeft(data[i]);
        if (left != nullptr) {
            std::cout << left->value << " -- "; 
        } else {
            std::cout << "-1 -- ";
        }
        Data* right = my_tree.childRight(data[i]);
        if (right != nullptr) {
            std::cout << right->value << " \n"; 
        } else {
            std::cout << "-1 \n";
        }
    }
    std::cout << "-------------------------\n";
}

TEST_CASE("remove") {
    templates::CAVLTree<Data, Compare> my_tree;
    Data data1("1", 1);
    Data data2("2", 2);
    std::vector<Data> data;
    data.push_back(data1);
    data.push_back(data2);
    CHECK(my_tree.add(&data1));
    CHECK(my_tree.add(&data2));
    //printTree(data, my_tree);
    CHECK(my_tree.remove(data1));
    CHECK(my_tree.find(data2));
}

TEST_CASE("updata") {
    int size = 10;
    templates::CAVLTree<Data, Compare> my_tree;
    std::vector<Data> data;
    for (int i = 0; i < size; ++i) {
        data.push_back(Data("name", i));
    }
    Data to_update("name", size / 2);
    for (int i = 0; i < size; ++i) {
        if (i == size/2) {
            my_tree.add(&to_update);
            continue;
        }
        CHECK(my_tree.add(&data[i]));
    }
    Data new_data("newData", size/2);
    CHECK(my_tree.update(&new_data));
    CHECK(my_tree.find(new_data)->key == "newData");
    Data errorData("data", size + 1);
    CHECK_FALSE(my_tree.update(&errorData));
}

TEST_CASE("just work") {
    int size = 10;
    templates::CAVLTree<Data, Compare> my_tree;
    std::vector<Data> data;
    for (int i = 0; i < size; ++i) {
        data.push_back(Data("name", i));
    }
    for (int i = 0; i < size; ++i) {
        CHECK(my_tree.add(&data[i]));
    }
    //printTree(data, my_tree);
    for (int i = 0; i < size; ++i) {
        CHECK_FALSE(my_tree.add(&data[i]));
    }
    for (int i = 0; i < size; ++i) {
        CHECK(my_tree.find(data[i]));
    }
    for (int i = 0; i < size; ++i) {
        if (i % 2 == 0) {
            CHECK(my_tree.remove(data[i]));
        }
    }
    for (int i = 0; i < size; ++i) {
        if (i % 2 == 0) {
            CHECK_FALSE(my_tree.remove(data[i]));
        } else {
            CHECK(my_tree.find(data[i]) != nullptr);
        }
    }
    //printTree(data, my_tree);
} 


TEST_CASE("balanced") {
    int size = 100;
    templates::CAVLTree<Data, Compare> my_tree;
    std::vector<Data> data;
    for (int i = 0; i < size; ++i) {
        data.push_back(Data("name", i));
    }
    for (int i = 0; i < size; ++i) {
        CHECK(my_tree.add(&data[i]));
        CHECK(my_tree.checkBalance());
    }
    //printTree(data, my_tree);
    for (int i = 0; i < size; ++i) {
        CHECK_FALSE(my_tree.add(&data[i]));
    }
    for (int i = 0; i < size; ++i) {
        CHECK(my_tree.find(data[i]));
    }
    for (int i = 0; i < size; ++i) {
        if (i % 2 == 0) {
            CHECK(my_tree.remove(data[i]));
            CHECK(my_tree.checkBalance());
        }
    }
    for (int i = 0; i < size; ++i) {
        if (i % 2 == 0) {
            CHECK_FALSE(my_tree.remove(data[i]));
        } else {
            CHECK(my_tree.find(data[i]) != nullptr);
        }
    }
    //printTree(data, my_tree);
}

TEST_CASE("after clear") {
    int size = 100;
    templates::CAVLTree<Data, Compare> my_tree;
    std::vector<Data> data;
    for (int i = 0; i < size; ++i) {
        data.push_back(Data("name", i));
    }
    for (int i = 0; i < size; ++i) {
        CHECK(my_tree.add(&data[i]));
        CHECK(my_tree.checkBalance());
    }
    //printTree(data, my_tree);
    for (int i = 0; i < size; ++i) {
        CHECK_FALSE(my_tree.add(&data[i]));
    }
    for (int i = 0; i < size; ++i) {
        CHECK(my_tree.find(data[i]));
    }
    for (int i = 0; i < size; ++i) {
        if (i % 2 == 0) {
            CHECK(my_tree.remove(data[i]));
            CHECK(my_tree.checkBalance());
        }
    }
    for (int i = 0; i < size; ++i) {
        if (i % 2 == 0) {
            CHECK_FALSE(my_tree.remove(data[i]));
        } else {
            CHECK(my_tree.find(data[i]) != nullptr);
        }
    }

    my_tree.clear();
    
    for (int i = 0; i < size; ++i) {
        data.push_back(Data("name", i));
    }
    for (int i = 0; i < size; ++i) {
        CHECK(my_tree.add(&data[i]));
        CHECK(my_tree.checkBalance());
    }
    //printTree(data, my_tree);
    for (int i = 0; i < size; ++i) {
        CHECK_FALSE(my_tree.add(&data[i]));
    }
    for (int i = 0; i < size; ++i) {
        CHECK(my_tree.find(data[i]));
    }
    for (int i = 0; i < size; ++i) {
        if (i % 2 == 0) {
            CHECK(my_tree.remove(data[i]));
            CHECK(my_tree.checkBalance());
        }
    }
    for (int i = 0; i < size; ++i) {
        if (i % 2 == 0) {
            CHECK_FALSE(my_tree.remove(data[i]));
        } else {
            CHECK(my_tree.find(data[i]) != nullptr);
        }
    }
}

TEST_CASE("error") {
    int size = 5;
    templates::CAVLTree<Data, Compare> my_tree;
    std::vector<Data> data;
    data.push_back(Data("name", 4));
    data.push_back(Data("name", 2));
    data.push_back(Data("name", 1));
    data.push_back(Data("name", 0));
    data.push_back(Data("name", 3));
    for (int i = 0; i < size; ++i) {
        CHECK(my_tree.add(&data[i]));
        CHECK(my_tree.checkBalance());
    }
    printTree(data, my_tree);
    for (int i = 0; i < size; ++i) {
        CHECK(my_tree.find(data[i]));
    }
    printTree(data, my_tree);
    for (int i = 0; i < size; ++i) {
        if (i % 2 == 0) {
            std::cout << data[i].value << "\n";
            CHECK(my_tree.remove(data[i]));
            printTree(data, my_tree);
            //CHECK(my_tree.checkBalance());
        }
    }
}

TEST_CASE("random") {
    int size = 100;
    templates::CAVLTree<Data, Compare> my_tree;
    std::vector<Data> data;
    for (int i = 0; i < size; ++i) {
        data.push_back(Data("name", i));
    }
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine e(seed);
    auto rng = std::default_random_engine {e};
    std::shuffle(data.begin(), data.end(), rng);
    for (int i = 0; i < data.size(); ++i) {
        std::cout << data[i].value << "\n";
    }
    std::cout << "------";
    for (int i = 0; i < size; ++i) {
        CHECK(my_tree.add(&data[i]));
        CHECK(my_tree.checkBalance());
    }
    for (int i = 0; i < size; ++i) {
        CHECK_FALSE(my_tree.add(&data[i]));
    }
    for (int i = 0; i < size; ++i) {
        CHECK(my_tree.find(data[i]));
    }
    for (int i = 0; i < size; ++i) {
        if (i % 2 == 0) {
            CHECK(my_tree.remove(data[i]));
            //CHECK(my_tree.checkBalance());
        }
    }
    for (int i = 0; i < size; ++i) {
        if (i % 2 == 0) {
            CHECK_FALSE(my_tree.remove(data[i]));
            CHECK(my_tree.find(data[i]) == nullptr);
        } else {
            CHECK(my_tree.find(data[i]) != nullptr);
        }
    }
    //printTree(data, my_tree);
}
