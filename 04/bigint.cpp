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

BigInt& BigInt::operator+(const BigInt& rhs) const {
    int carry = 0;
    size_t position = (rhs.getNumber().cend() - rhs.getNumber().cbegin()) - 1;
    std::cout << position << std::endl;
    BigInt result; // RETURN LOCAL STACK VALUE!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

    for (auto it = rhs.getNumber().crbegin(); it != rhs.getNumber().crend(); ++it) {
        int tempLHS = this->getNumber(position) - '0';
        int tempRHS = *it - '0';

        int sum = tempLHS + tempRHS + carry;
        carry = sum / 10;
        int current = sum % 10;
        (result.number)[position] = current;        
        
        position--;
    }
    return result;
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