#include <iostream>
#include <cstdlib>
#include "parent.h"


int main(void) {
    setenv("CHILD_BINARY_PATH", "/mai-os-labs/lab1/src/child", 1);
    std::string filename, filename_out;
    std::cout << "Введите имя файла: ";
    std::cin >> filename;
    std::cout << "Введите имя файла для вывода: ";
    std::cin >> filename_out;
    Parent(filename.c_str(), filename_out.c_str());
    return 0;
}