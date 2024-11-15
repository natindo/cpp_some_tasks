#include "bigint.h"
#include <iostream>
#include <iterator>
#include <new>
#include <string>

BigInt::BigInt() {
    try {
        number = new std::string();
    } catch (std::bad_alloc const&) {
        std::cout << "some problem with memory" << std::endl;
    }
}

BigInt::BigInt(int32_t input) {
    try {
        number = new std::string(std::to_string(input));
    } catch (std::bad_alloc const&) {
        std::cout << "some problem with memory" << std::endl;
    }
    *number = std::to_string(input);
}

BigInt::BigInt(std::string input) {
    try {
        number = new std::string(input);
    } catch (std::bad_alloc const&) {
        std::cout << "some problem with memory" << std::endl;
    }
    *number = input;
}

std::string& BigInt::getNumber() const {
    return *this->number;
}

char BigInt::getNumber(size_t position) const {
    return (*number)[position];
}

BigInt& BigInt::operator+(const BigInt& rhs) {
    int carry = 0;
    size_t position = (rhs.getNumber().cend() - rhs.getNumber().cbegin()) / sizeof(char);
    // std::cout << position << std::endl;

    for (auto it = rhs.getNumber().cend(); it != rhs.getNumber().cbegin(); --it) {
        int tempLHS = static_cast<int>(this->getNumber(position));
        int tempRHS = static_cast<int>(*it);

        int sum = tempLHS + tempRHS + carry;
        carry = sum / 10;
        int current = sum % 10;
        (*this->number)[position] = current;        
        
        position--;
    }
    return *this;
}

void BigInt::operator=(const BigInt& rhs) {
    *this->number = rhs.getNumber();
}

std::ostream& operator<<(std::ostream& out, const BigInt& bigint) {
    out << bigint.getNumber();
    return out;
}

BigInt::~BigInt() {
    delete number;
}

int main () {
    BigInt a (1);
    BigInt b ("123456789012345678901234567890");
    BigInt c;

    std::cout << a << std::endl;
    std::cout << b << std::endl;
    std::cout << c << std::endl;

    std::cout << b + a << std :: endl;

    return 0;
}