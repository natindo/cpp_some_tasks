#include "bigint.h"
#include <cstddef>
#include <iostream>
#include <new>
#include <cstring>
#include <string>

BigInt::BigInt() : digit_(nullptr), size_(0) {}

BigInt::BigInt(int32_t input) : digit_(nullptr), size_(0), isNegative_(input < 0) {
    int32_t value = input;
    if (isNegative_) {
        value = -value;
    }
    std::string str = std::to_string(value);
    size_ = str.size();
    allocate(size_);

    for (size_t i = 0; i < size_; ++i) {
        digit_[i] = str[size_ - i - 1];
    }
}

BigInt::BigInt(const BigInt& rhs) : digit_(nullptr), size_(rhs.size_) {
    copy(rhs);
}

BigInt::BigInt(const std::string& input) : digit_(nullptr), size_(0) {
    size_ = input.size();
    allocate(size_);
    for (size_t i = 0; i < size_; ++i) {
        digit_[i] = input[size_ - i - 1];
    }
}

BigInt::BigInt(BigInt&& rhs) : digit_(nullptr), size_(0) {
    digit_ = rhs.digit_;
    size_ = rhs.size_;

    rhs.digit_ = nullptr;
    rhs.size_ = 0;
}

BigInt& BigInt::operator=(BigInt&& rhs) {
    if (this != &rhs) {
        deallocate();
        
    digit_ = rhs.digit_;
    size_ = rhs.size_;

    rhs.digit_ = nullptr;
    rhs.size_ = 0;
    }
    return *this;
}

void BigInt::copy(const BigInt& rhs) {
    if (this != &rhs) {
        deallocate();
        allocate(size_);
        std::memcpy(digit_, rhs.digit_, rhs.size_ * sizeof(char));
    }
}

void BigInt::allocate(size_t newSize) {
    digit_ = new char[newSize];
    size_ = newSize;
}

void BigInt::deallocate() {
    delete[] digit_;
    digit_ = nullptr;
    size_ = 0;
}

int BigInt::compareAbs(const BigInt& rhs) const {
    if (size_ > rhs.size_) return 1;
    if (size_ < rhs.size_) return -1;
    for (size_t i = size_; i > 0; --i) {
        if (digit_[i - 1] > rhs.digit_[i - 1]) return 1;
        if (digit_[i - 1] < rhs.digit_[i - 1]) return -1;
    }
    return 0;
}

BigInt& BigInt::operator=(const BigInt& rhs) {
    if (this != &rhs) {
        copy(rhs);
    }
    return *this;
}

BigInt BigInt::operator*(const BigInt& rhs) const {
    if ((size_ == 1 && digit_[0] == '0') || (rhs.size_ == 1 && rhs.digit_[0] == '0')) {
        return BigInt(0);
    }

    size_t resultSize = size_ + rhs.size_;
    char* resultString = new char[resultSize];

    memset(resultString, 0, resultSize);

     for (size_t i = 0; i < size_; ++i) {
        int carry = 0;
        int n1 = digit_[i] - '0';

        for (size_t j = 0; j < rhs.size_; ++j) {
            int n2 = rhs.digit_[j] - '0';

            int sum = (resultString[i + j] - '0') + n1 * n2 + carry;
            carry = sum / 10;
            resultString[i + j] = (sum % 10) + '0';
        }

        if (carry > 0) {
            resultString[i + rhs.size_] += carry;
            if (resultString[i + rhs.size_] > '9') {
                int tempCarry = (resultString[i + rhs.size_] - '0') / 10;
                resultString[i + rhs.size_] = ((resultString[i + rhs.size_] - '0') % 10) + '0';
                size_t k = i + rhs.size_ + 1;
                while (tempCarry > 0) {
                    if (k >= resultSize) {
                        char* newResultDigits = new char[resultSize + 1];
                        std::memcpy(newResultDigits, resultString, resultSize * sizeof(char));
                        newResultDigits[resultSize] = '0';
                        delete[] resultString;
                        resultString = newResultDigits;
                        resultSize++;
                    }
                    int newSum = (resultString[k] - '0') + tempCarry;
                    tempCarry = newSum / 10;
                    resultString[k] = (newSum % 10) + '0';
                    k++;
                }
            }
        }
    }

    size_t actualSize = resultSize;
    while (actualSize > 1 && resultString[actualSize - 1] == '0') {
        --actualSize;
    }

    BigInt result;
    result.deallocate();
    result.allocate(actualSize);
    std::memcpy(result.digit_, resultString, actualSize * sizeof(char));
    result.size_ = actualSize;

    delete[] resultString;

    return result;
}

BigInt BigInt::operator-() const {
    BigInt result(*this);
    if (size_ != 0) {
        result.isNegative_ = !isNegative_;
    }
    return result;
}

BigInt BigInt::operator+(const BigInt& rhs) const {
    if (isNegative_ != rhs.isNegative_) {
        if (isNegative_) {
            // (-a) + b == b - a
            return rhs - (-(*this));
        } else {
            // a + (-b) == a - b
            return (*this) - (-rhs);
        }
    }

    size_t maxSize = std::max(size_, rhs.size_);
    size_t resultSize = maxSize + 1;
    int carry = 0;
    size_t i = 0;
    char* resultString = new char[resultSize];

    for (; i < maxSize; ++i) {
        int tempLHS = i < size_ ? digit_[i] - '0' : 0;
        int tempRHS = i < rhs.size_ ? rhs.digit_[i] - '0' : 0;

        int sum = tempLHS + tempRHS + carry;
        carry = sum / 10;
        int current = sum % 10;
        resultString[i] = current + '0';
    }
    
    if (carry) {
        resultString[i++] = carry + '0';
    }

    BigInt result;
    result.deallocate();
    result.isNegative_ = isNegative_;
    result.allocate(i);
    std::memcpy(result.digit_, resultString, i * sizeof(char));
    result.size_ = i;
    delete[] resultString;

    return result;
}

BigInt BigInt::operator-(const BigInt& rhs) const {
    if (isNegative_ != rhs.isNegative_) {
        BigInt result = (*this) + (-rhs);
        result.isNegative_ = isNegative_;
        return result;
    }

    int cmp = compareAbs(rhs);
    if (cmp == 0) {
        return BigInt(0);
    }

    const BigInt *minuend = this;
    const BigInt *subtrahend = &rhs;
    bool resultNegative = false;

    if (cmp < 0) {
        std::swap(minuend, subtrahend);
        resultNegative = !isNegative_;
    } else {
        resultNegative = isNegative_;
    }

    size_t resultSize = minuend->size_;
    char* resultString = new char[resultSize];
    int borrow = 0;

    for (size_t i = 0; i < resultSize; ++i) {
        int tempMinuend = minuend->digit_[i] - '0';
        int tempSubtrahend = i < subtrahend->size_ ? subtrahend->digit_[i] - '0' : 0;

        int diff = tempMinuend - tempSubtrahend - borrow;
        if (diff < 0) {
            diff += 10;
            borrow = 1;
        } else {
            borrow = 0;
        }

        resultString[i] = diff + '0';
    }

    size_t actualSize = resultSize;
    while (actualSize > 1 && resultString[actualSize - 1] == '0') {
        --actualSize;
    }

    BigInt result;
    result.isNegative_ = resultNegative;
    result.allocate(actualSize);
    std::memcpy(result.digit_, resultString, actualSize * sizeof(char));
    result.size_ = actualSize;

    delete[] resultString;
    return result;
}


BigInt BigInt::operator-(const int32_t rhs) const {
    BigInt rhsBigInt(rhs);
    return (*this) - rhsBigInt;
}

std::ostream& operator<<(std::ostream& out, const BigInt& bigint) {
    for (size_t i = 0; i < bigint.size_; ++i) {
        out << bigint.digit_[bigint.size_ - 1 - i];
    }
    return out;
}

BigInt::~BigInt() {
    deallocate();
}


BigInt createBigInt() {
    BigInt temp("987654321098765432109876543210");
    return temp; // Здесь будет использован конструктор перемещения
}

int main() {
    BigInt a(1);
    BigInt b("12345678901234567890");
    BigInt c("98765432109876543210");

    std::cout << "a = " << a << std::endl;
    std::cout << "b = " << b << std::endl;
    std::cout << "c = " << c << std::endl;

    BigInt d = a + b;
    // std::cout << "a + b = " << d << std::endl;

    BigInt e = std::move(d);
    std::cout << "e = " << e << std::endl;

    // c = std::move(e);
    // std::cout << "c (after move) = " << c << std::endl;

    // BigInt f = b * c;
    // std::cout << "b * c = " << f << std::endl;

    return 0;
}