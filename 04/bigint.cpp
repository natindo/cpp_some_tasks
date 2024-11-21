#include "bigint.h"
#include <cstddef>
#include <iostream>
#include <iterator>
#include <new>
#include <cstring>
#include <string>

BigInt::BigInt() : digit(nullptr), size(0) {}

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

BigInt& BigInt::operator+(const BigInt& rhs) const {

    try {
        int carry = 0;
        size_t position = (rhs.getNumber().cend() - rhs.getNumber().cbegin()) - 1;
        std::cout << position << std::endl;
        BigInt* value = new BigInt();
        for (auto it = rhs.getNumber().crbegin(); it != rhs.getNumber().crend(); ++it) {
            int tempLHS = this->getNumber(position) - '0';
            int tempRHS = *it - '0';

            int sum = tempLHS + tempRHS + carry;
            carry = sum / 10;
            int current = sum % 10;
            (value->number)[position] = current;        
            
            position--;
        }
        return *value;
    } catch (std::bad_alloc const&) {
        std::cout << "some problem with memory, error in operation plus" << std::endl;
        return ;
    }
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
    BigInt b ("123");
    BigInt c;

    std::cout << a << std::endl;
    std::cout << b << std::endl;
    std::cout << c << std::endl;

    std::cout << a + b << std :: endl;

    return 0;
}