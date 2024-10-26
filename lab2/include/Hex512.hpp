#ifndef HEX_H
#define HEX_H
#include <vector>
#include <string>


class Hex512 {
private:
    std::vector<unsigned int> digits;
    void trim_leading_zeros(); 
    unsigned long long to_unsigned_long_long() const;
    bool is_zero() const;
public:
    Hex512();
    Hex512(const std::string& line);
    ~Hex512();

    Hex512 operator+(const Hex512 &other) const;
    Hex512 operator-(const Hex512 &other) const;
    Hex512& operator=(const Hex512 &other);
    bool operator>(const Hex512 &other) const;
    bool operator>=(const Hex512 &other) const;
    bool operator==(const Hex512& other) const;
    Hex512& operator+=(const Hex512 &other);
    Hex512 operator/(const Hex512 &divisor) const;
    void print_num() const;
};


#endif