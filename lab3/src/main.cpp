#include <iostream>
#include <cstdlib>
#include "parent.h"


int main(void) {
    std::string filename;
    std::cout << "Введите имя файла: ";
    std::cin >> filename;
    std::ostream& output = std::cout;
    Parent(filename.c_str(), output);
    return 0;
}