#include "parent.h"
#include <iostream>
#include <sstream>


int main(void) {
    std::string filename;
    std::cout << "Введите имя файла: ";
    std::cin >> filename;
    std::ostream& out_stream = std::cout;
    Parent(filename.c_str(), out_stream);
    return 0;
}