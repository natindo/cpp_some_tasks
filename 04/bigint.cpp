#include "bigint.h"
#include <cstddef>
#include <cstdint>
#include <iostream>
// #include <new>
#include <cstring>
#include <string>

BigInt::BigInt()
    : digit_(nullptr)
    , size_(0)
    , isNegative_(false)
{
}

BigInt::BigInt(int32_t input)
    : digit_(nullptr)
    , size_(0)
    , isNegative_(input < 0)
{
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

BigInt::BigInt(const BigInt& rhs)
    : digit_(nullptr)
    , size_(rhs.size_)
    , isNegative_(rhs.isNegative_)
{
    copy(rhs);
}

BigInt::BigInt(const std::string& input)
    : digit_(nullptr)
    , size_(0)
    , isNegative_(false)
{
    size_t start = 0;
    if (input[0] == '-') {
        isNegative_ = true;
        start = 1;
    }

    std::string str = input.substr(start);
    while (str.size() > 1 && str[0] == '0') {
        str.erase(str.begin());
    }

    size_ = str.size();
    allocate(size_);

    for (size_t i = 0; i < size_; ++i) {
        char c = str[str.size() - 1 - i];
        if (c < '0' || c > '9') {
            deallocate();
            throw std::invalid_argument("Character must be a digit!");
        }
        digit_[i] = c;
    }

    if (isZero()) {
        isNegative_ = false;
    }
}

BigInt::BigInt(BigInt&& rhs)
    : digit_(nullptr)
    , size_(0)
{
    digit_ = rhs.digit_;
    size_ = rhs.size_;
    isNegative_ = rhs.isNegative_;

    rhs.digit_ = nullptr;
    rhs.size_ = 0;
    rhs.isNegative_ = false;
}

BigInt& BigInt::operator=(const BigInt& rhs)
{
    if (this != &rhs) {
        copy(rhs);
    }
    return *this;
}

BigInt& BigInt::operator=(BigInt&& rhs)
{
    if (this != &rhs) {
        deallocate();

        digit_ = rhs.digit_;
        size_ = rhs.size_;
        isNegative_ = rhs.isNegative_;

        rhs.digit_ = nullptr;
        rhs.size_ = 0;
        rhs.isNegative_ = false;
    }
    return *this;
}

void BigInt::copy(const BigInt& rhs)
{
    if (this != &rhs) {
        deallocate();
        size_ = rhs.size_;
        isNegative_ = rhs.isNegative_;
        allocate(size_);
        std::memcpy(digit_, rhs.digit_, rhs.size_ * sizeof(char));
    }
}

void BigInt::allocate(size_t newSize)
{
    digit_ = new char[newSize];
    size_ = newSize;
}

void BigInt::deallocate()
{
    delete[] digit_;
    digit_ = nullptr;
    size_ = 0;
}

int BigInt::compareAbs(const BigInt& rhs) const
{
    if (size_ > rhs.size_)
        return 1;
    if (size_ < rhs.size_)
        return -1;
    for (size_t i = size_; i > 0; --i) {
        if (digit_[i - 1] > rhs.digit_[i - 1])
            return 1;
        if (digit_[i - 1] < rhs.digit_[i - 1])
            return -1;
    }
    return 0;
}

bool BigInt::isZero() const
{
    if (size_ == 0)
        return true;
    if (size_ == 1 && digit_[0] == '0')
        return true;
    for (size_t i = 0; i < size_; ++i) {
        if (digit_[i] != '0') {
            return false;
        }
    }
    return true;
}

BigInt BigInt::operator+(const BigInt& rhs) const
{
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
        int tempLHS = (i < size_) ? digit_[i] - '0' : 0;
        int tempRHS = (i < rhs.size_) ? rhs.digit_[i] - '0' : 0;

        int sum = tempLHS + tempRHS + carry;
        carry = sum / 10;
        int current = sum % 10;
        resultString[i] = current + '0';
    }

    if (carry) {
        resultString[i++] = carry + '0';
    }

    BigInt result;

    while (i > 1 && resultString[i - 1] == '0') {
        i--;
    }

    result.deallocate();
    result.isNegative_ = isNegative_;
    result.allocate(i);
    std::memcpy(result.digit_, resultString, i * sizeof(char));
    result.size_ = i;
    delete[] resultString;

    if (result.isZero()) {
        result.isNegative_ = false;
    }

    return result;
}

BigInt BigInt::operator+(int32_t rhs) const
{
    BigInt rhsBigInt(rhs);
    return (*this) + rhsBigInt;
}

BigInt BigInt::operator-() const
{
    BigInt result(*this);
    if (!result.isZero()) {
        result.isNegative_ = !isNegative_;
    }
    return result;
}

BigInt BigInt::operator-(const BigInt& rhs) const
{
    // a - (-b) = a + b
    // (-a) - b = -(a + b)
    if (isNegative_ != rhs.isNegative_) {
        return (*this) + (-rhs);
    }

    int cmp = compareAbs(rhs);
    if (cmp == 0) {
        return BigInt(0);
    }

    const BigInt* minuend = this;
    const BigInt* subtrahend = &rhs;
    bool resultNegative = false;

    if (cmp < 0) {
        std::swap(minuend, subtrahend);
        resultNegative = minuend->isNegative_; //затестить
        // resultNegative = !isNegative_;
    } else {
        resultNegative = isNegative_;
    }

    size_t resultSize = minuend->size_;
    char* resultString = new char[resultSize];
    int borrow = 0;

    for (size_t i = 0; i < resultSize; ++i) {
        int tempMinuend = minuend->digit_[i] - '0';
        int tempSubtrahend = (i < subtrahend->size_) ? subtrahend->digit_[i] - '0' : 0;

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

    if (result.isZero()) {
        result.isNegative_ = false;
    }

    return result;
}

BigInt BigInt::operator-(int32_t rhs) const
{
    BigInt rhsBigInt(rhs);
    return (*this) - rhsBigInt;
}

BigInt BigInt::operator*(const BigInt& rhs) const
{
    if (isZero() || rhs.isZero()) {
        return BigInt(0);
    }

    size_t resultSize = size_ + rhs.size_;
    char* resultString = new char[resultSize];

    memset(resultString, '0', resultSize);

    for (size_t i = 0; i < size_; ++i) {
        int carry = 0;
        int n1 = digit_[i] - '0';

        for (size_t j = 0; j < rhs.size_; ++j) {
            int n2 = (j < rhs.size_) ? (rhs.digit_[j] - '0') : 0;

            int sum = (resultString[i + j] - '0') + n1 * n2 + carry;
            carry = sum / 10;
            resultString[i + j] = (sum % 10) + '0';
        }
    }

    while (resultSize > 1 && resultString[resultSize - 1] == '0') {
        --resultSize;
    }

    BigInt result;
    result.deallocate();
    result.allocate(resultSize);
    std::memcpy(result.digit_, resultString, resultSize * sizeof(char));
    result.isNegative_ = (isNegative_ != rhs.isNegative_);
    result.size_ = resultSize;

    delete[] resultString;

    if (result.isZero()) {
        result.isNegative_ = false;
    }

    return result;
}

BigInt BigInt::operator*(int32_t rhs) const
{
    BigInt rhsBigInt(rhs);
    return (*this) * rhsBigInt;
}

bool BigInt::operator==(const BigInt& rhs) const
{
    if (isNegative_ != rhs.isNegative_) {
        return false;
    }
    if (size_ != rhs.size_) {
        return false;
    }
    for (size_t i = 0; i < size_; i++) {
        if (digit_[i] != rhs.digit_[i]) {
            return false;
        }
    }
    return true;
}

bool BigInt::operator!=(const BigInt& rhs) const
{
    return !(*this == rhs);
}

bool BigInt::operator<(const BigInt& rhs) const
{
    if (isNegative_ && !rhs.isNegative_) {
        return true;
    }
    if (!isNegative_ && rhs.isNegative_) {
        return false;
    }

    int cmp = compareAbs(rhs);

    if (cmp == 0) {
        return false;
    }

    if (isNegative_) {
        return cmp > 0;
    } else {
        return cmp < 0;
    }
}

bool BigInt::operator<=(const BigInt& rhs) const
{
    return !(*this > rhs);
}

bool BigInt::operator>(const BigInt& rhs) const
{
    return rhs < *this;
}

bool BigInt::operator>=(const BigInt& rhs) const
{
    return !(*this < rhs);
}

std::ostream& operator<<(std::ostream& out, const BigInt& bigint)
{
    if (bigint.isNegative_ && !bigint.isZero()) {
        out << '-';
    }
    for (size_t i = 0; i < bigint.size_; ++i) {
        out << bigint.digit_[bigint.size_ - 1 - i];
    }
    return out;
}

BigInt::~BigInt()
{
    deallocate();
}

BigInt createBigInt()
{
    BigInt temp("987654321098765432109876543210");
    return temp; // Здесь будет использован конструктор перемещения
}
