#include "mm.h"
#include "hash.h"
#include "avltree.h"
#include "sort.h"
#include <cstring>

#include<iostream>
#include<vector>
#include <chrono>
#include <fstream>

struct Data {
    Data() = default;
    Data(std::string value, std::string key) : value(value), key(key){}
    std::string value;
    std::string key;
};

unsigned int HashFunc(const Data* data) {
    uint64_t P = 1;
    uint64_t hash = 0;
    for (size_t i = 0; i < data->key.size(); ++i) {
        hash += (data->key.at(i) - 'a') * P;
        P *= 31;
    }
    return hash;
}


int Compare(const Data* a, const Data* b) {
    const std::string& a_str = a->value;
    const std::string& b_str = b->value;
    int delta = 0;
    size_t iter = 0;
    while (iter < a_str.size() && iter < b_str.size()) {
        delta = a_str[iter] - b_str[iter];
        if (delta != 0) {
            return a_str[iter] - b_str[iter];
        }
        ++iter;
    }
    return a_str.size() - b_str.size();
}


std::string gen_random(const int len) {
    static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";
    std::string tmp_s;
    tmp_s.reserve(len);

    for (int i = 0; i < len; ++i) {
        tmp_s += alphanum[rand() % (sizeof(alphanum) - 1)];
    }
    
    return tmp_s;
}

void insertHash(lab618::CHash<Data, &HashFunc, &Compare>& hash, std::vector<Data*>& data, bool flag=true);
void insertTree(templates::CAVLTree<Data, Compare>& tree, std::vector<Data*>& data, bool flag=true);
void insertArray(Data** array, std::vector<Data*>& data, bool flag=true);
void findAllHash(lab618::CHash<Data, &HashFunc, &Compare>& hash, std::vector<Data*>& data, bool flag=true);
void findAllTree(templates::CAVLTree<Data, Compare>& tree, std::vector<Data*>& data, bool flag=true);
void findAllArray(Data** array, int size, std::vector<Data*>& data, bool flag=true);
void deleteHash(lab618::CHash<Data, &HashFunc, &Compare>& hash);
void deleteArray(Data** array);
void deleteTree(templates::CAVLTree<Data, Compare>& tree);
void clearByOneHash(lab618::CHash<Data, &HashFunc, &Compare>& hash, std::vector<Data*>& data);
void clearByOneTree(templates::CAVLTree<Data, Compare>& tree, std::vector<Data*>& data);

std::ofstream init;
std::ofstream findAll;
std::ofstream findOther;
std::ofstream clear;
std::ofstream pop;

int main() {
    init.open("../results/init.csv");
    findAll.open("../results/findAll.csv");
    findOther.open("../results/findOther.csv");
    clear.open("../results/clear.csv");
    pop.open("../results/pop.csv");
    init << "Hash,Tree,Array\n";
    findAll << "Hash,Tree,Array\n";
    findOther << "Hash,Tree,Array\n";
    clear << "Hash,Tree,Array\n";
    pop << "Hash,Tree\n";
    for (int N = 1000; N < 1000000; N += 50000) {
        std::cout << N << "\n";
        std::vector<Data*> data(N);
        for (long unsigned int i = 0; i < data.size(); ++i) {
            int len1 = 7 + (rand() % 14);
            int len2 = 7 + (rand() % 14);
            data[i] = new Data(gen_random(len1), gen_random(len2));
        }

        std::vector<Data*> data_to_find(2 * N);
        for (long unsigned int i = 0; i < data_to_find.size(); ++i) {
            int len1 = 7 + (rand() % 14);
            int len2 = 7 + (rand() % 14);
            data_to_find[i] = new Data(gen_random(len1), gen_random(len2));
        }


        lab618::CHash<Data, &HashFunc, &Compare> hash(N, 1000); 
        templates::CAVLTree<Data, Compare> tree;
        Data** array = new Data*[N];

        init << N << ",";
        insertHash(hash, data);
        insertTree(tree, data);
        insertArray(array, data);

        findAll << N << ",";
        findAllHash(hash, data);
        findAllTree(tree, data);
        findAllArray(array, N, data);

        findOther << N << ",";
        findAllHash(hash, data_to_find, false);
        findAllTree(tree, data_to_find, false);
        findAllArray(array, N, data_to_find, false);

        clear << N << ",";
        deleteHash(hash);
        deleteTree(tree);
        deleteArray(array);

        insertHash(hash, data, false);
        insertTree(tree, data, false);

        pop << N << ",";
        clearByOneHash(hash, data);
        clearByOneTree(tree, data);

        for (long unsigned int i = 0; i < data.size(); ++i) {
            delete data[i];
        }

    }
    init.close();
    findAll.close();
    findOther.close();
    return 0;
}

void insertHash(lab618::CHash<Data, &HashFunc, &Compare>& hash, std::vector<Data*>& data, bool flag) {
    auto start_time = std::chrono::steady_clock::now();
    for (long unsigned int i = 0; i < data.size(); ++i) {
        hash.add(data[i]);
    }
    auto end_time = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time);
    if (flag) init << elapsed.count() << ",";
}

void findAllHash(lab618::CHash<Data, &HashFunc, &Compare>& hash, std::vector<Data*>& data, bool flag) {
    auto start_time = std::chrono::steady_clock::now();
    for (long unsigned int i = 0; i < data.size(); ++i) {
        hash.find(*data[i]);
    }
    auto end_time = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time);
    if (flag) {
        findAll << elapsed.count() << ",";
    } else {
        findOther << elapsed.count() << ",";
    }
}

void deleteHash(lab618::CHash<Data, &HashFunc, &Compare>& hash) {
    auto start_time = std::chrono::steady_clock::now();
    hash.clear();
    auto end_time = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time);
    clear << elapsed.count() << ",";
}

void clearByOneHash(lab618::CHash<Data, &HashFunc, &Compare>& hash, std::vector<Data*>& data) {
    auto start_time = std::chrono::steady_clock::now();
    for (long unsigned int i = 0; i < data.size(); ++i) {
        hash.remove(*data[i]);
    }
    auto end_time = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time);
    pop << elapsed.count() << ",";
}

void insertTree(templates::CAVLTree<Data, Compare>& tree, std::vector<Data*>& data, bool flag) {
    auto start_time = std::chrono::steady_clock::now();
    for (long unsigned int i = 0; i < data.size(); ++i) {
        tree.add(data[i]);
    }
    auto end_time = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time);
    if (flag) init << elapsed.count() << ",";
}

void findAllTree(templates::CAVLTree<Data, Compare>& tree, std::vector<Data*>& data, bool flag) {
    auto start_time = std::chrono::steady_clock::now();
    for (long unsigned int i = 0; i < data.size(); ++i) {
        tree.find(*data[i]);
    }
    auto end_time = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time);
    if (flag) {
        findAll << elapsed.count() << ",";
    } else {
        findOther << elapsed.count() << ",";
    }
}

void deleteTree(templates::CAVLTree<Data, Compare>& tree) {
    auto start_time = std::chrono::steady_clock::now();
    tree.clear();
    auto end_time = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time);
    clear << elapsed.count() << ",";
}

void clearByOneTree(templates::CAVLTree<Data, Compare>& tree, std::vector<Data*>& data) {
    auto start_time = std::chrono::steady_clock::now();
    for (long unsigned int i = 0; i < data.size(); ++i) {
        tree.remove(*data[i]);
    }
    auto end_time = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time);
    pop << elapsed.count() << "\n";
}

void insertArray(Data** array, std::vector<Data*>& data, bool flag) {
    auto start_time = std::chrono::steady_clock::now();
    for (long unsigned int i = 0; i < data.size(); ++i) {
        array[i] = data[i];
    }
    templates::mergeSort<Data>(array, data.size(), Compare);
    auto end_time = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time);
    if (flag) init << elapsed.count() << "\n";
}

bool findInArray(Data** array, Data* data, int len) {
    int left = 0;
    int right = len;
    int compare_result = 0;
    int middle = 0;
    while (right > left + 1) {
        middle = (right + left) / 2;
        compare_result = Compare(array[middle], data);
        if (compare_result == 0) {
            return true;
        }
        if (compare_result < 0) {
            left = middle;
        } else {
            right = middle;
        }
    }
    return -1;
    return Compare(array[left], data) == 0;
}

void findAllArray(Data** array, int size, std::vector<Data*>& data, bool flag) {
    auto start_time = std::chrono::steady_clock::now();
    for (long unsigned int i = 0; i < data.size(); ++i) {
        findInArray(array, data[i], size);
    }
    auto end_time = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time);
    if (flag) {
        findAll << elapsed.count() << "\n";
    } else {
        findOther << elapsed.count() << "\n";
    }
}

void deleteArray(Data** array) {
    auto start_time = std::chrono::steady_clock::now();
    delete[] array;
    array = nullptr;
    auto end_time = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time);
    clear << elapsed.count() << "\n";
}