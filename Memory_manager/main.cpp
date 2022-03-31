#include <iostream>
#include <string>
#include "mm.h"

struct Data{
public:
    Data() {}

    ~Data() {}

    void setStr(std::string text) {
        str = text;
        value = 10;
    }

private:
    int value; 
    std::string str;
};

int main() {
    lab618::CMemoryManager<Data> manager(10, true);
    Data* data = manager.newObject();
    data->setStr("My_first");
    return 0;
}
