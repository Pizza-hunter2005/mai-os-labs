#include <iostream>
#include <cstdlib>
#include "parent.h"


int main(void) {
    setenv("CHILD_BINARY_PATH", "/root/mai-os-labs/lab1/build/child", 1);
    std::string filename;
    std::cout << "Введите имя файла: ";
    std::cin >> filename;
    std::ostream& output = std::cout;
    Parent(filename.c_str(), output);
    return 0;
}