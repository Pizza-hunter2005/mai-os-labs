#include <iostream>
#include <string>
#include <sstream>
#include "child.h"


void Child() {
    float sum = 0.0f;
    float num = 0.0f;
    std::string line;
    while(std::cin >> num) {
        sum += num;
    }
    std::cout << sum << std::endl;
}

int main() {
    Child();
    return 0;
}

