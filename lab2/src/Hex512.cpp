#include "Hex512.hpp"
#include <iostream>
#include <stdexcept>


Hex512::Hex512() : digits(1, 0) {}

Hex512::Hex512(const std::string &line) {
    // Храним цифры числа в векторе в обратном порядке для удобства сложения (также буквенные обозначения конвертируем в числа)
    for (auto it = line.rbegin(); it != line.rend(); ++it) {
        if (*it >= '0' && *it <= '9') {
            digits.push_back(*it - '0');
        } else if (*it >= 'A' && *it <= 'F') {
            digits.push_back(*it - 'A' + 10);
        } else if (*it >= 'a' && *it <= 'f') {
            digits.push_back(*it - 'a' + 10);
        } else {
            throw std::invalid_argument("Invalid digit for base-16 number");
        }
    }
    this->trim_leading_zeros();
}

Hex512::~Hex512() = default;

Hex512 Hex512::operator+(const Hex512 &other) const {
    Hex512 result;
    size_t max_size = std::max(digits.size(), other.digits.size());
    result.digits.resize(max_size + 1, 0);

    unsigned char carry = 0;
    for (size_t i = 0; i < max_size; ++i) {
        unsigned char a = (i < digits.size()) ? digits[i] : 0;
        unsigned char b = (i < other.digits.size()) ? other.digits[i] : 0;
        unsigned char sum = a + b + carry;
        result.digits[i] = sum % 16;
        carry = sum / 16;
    }

    if (carry != 0) {
        result.digits[max_size] = carry; 
    } else {
        result.digits.resize(max_size);
    }

    result.trim_leading_zeros();
    return result;
}

Hex512 Hex512::operator-(const Hex512 &other) const {
    if (other > *this) {
        throw std::underflow_error("Result of subtraction is negative");
    }
    Hex512 result;
    result.digits.resize(digits.size(), 0);

    unsigned char borrow = 0;
    for (size_t i = 0; i < digits.size(); ++i) {
        unsigned char a = digits[i];
        unsigned char b = (i < other.digits.size()) ? other.digits[i] : 0;
        unsigned char diff;
        if (a < b + borrow) {
            diff = (a + 16) - b - borrow;
            borrow = 1;
        } else {
            diff = a - b - borrow;
            borrow = 0;
        }

        result.digits[i] = diff;
    }

    result.trim_leading_zeros();
    return result;
}

Hex512& Hex512::operator=(const Hex512 &other) {
    if (this != &other) {
        digits = other.digits;
    }
    return *this;
}

bool Hex512::operator==(const Hex512& other) const {
    return digits == other.digits;
}

bool Hex512::operator>(const Hex512 &other) const {
    if (digits.size() > other.digits.size()) {
        return true;
    }
    if (digits.size() < other.digits.size()) {
        return false;
    }

    for (int i = digits.size() - 1; i >= 0; --i) {
        if (digits[i] > other.digits[i]) {
            return true;
        }
        if (digits[i] < other.digits[i]) {
            return false;
        }
    }
    return false;
}

bool Hex512::operator>=(const Hex512 &other) const {
    if (*this == other || *this > other) {return true;}
    return false;
}

Hex512& Hex512::operator+=(const Hex512 &other) {
    *this = *this + other;
    return *this;
}


Hex512 Hex512::operator/(const Hex512 &divisor) const {
    if (divisor.is_zero()) {
        throw std::invalid_argument("Division by zero!");
    }

    Hex512 result;
    result.digits.resize(digits.size(), 0);
    Hex512 current; 
    for (int i = digits.size() - 1; i >= 0; --i) {
        current.digits.insert(current.digits.begin(), digits[i]);
        current.trim_leading_zeros();
        int count = 0;
        while (current >= divisor) {
            current = current - divisor;
            count++;
        }

        result.digits[i] = count;
    }

    result.trim_leading_zeros();
    return result;
}

bool Hex512::is_zero() const {
    for (const auto& digit : digits) {
        if (digit != 0) {
            return false;
        }
    }
    return true;
}

void Hex512::trim_leading_zeros() {
    while (digits.size() > 1 && digits.back() == 0) {
        digits.pop_back();
    }
}

void Hex512::print_num() const {
    if (digits.empty()) {
        std::cout << "0";
        return;
    }
    for (auto it = digits.rbegin(); it != digits.rend(); ++it) {
        if (*it > 9) {
            std::cout << static_cast<char>(*it - 10 + 'A');
        } else {
            std::cout << static_cast<int>(*it);
        }
    }
    std::cout << std::endl;
}
