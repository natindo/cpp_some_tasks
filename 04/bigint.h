#pragma once
#include <cstddef>
#include <ostream>
#include <string>

class BigInt {

public:
    BigInt();
    BigInt(int32_t input);
    BigInt(const std::string& input);
    BigInt(const BigInt& rhs);
    BigInt(BigInt&& rhs);

    BigInt& operator=(const BigInt& rhs);
    BigInt& operator=(BigInt&& rhs);

    BigInt operator+(const BigInt& rhs) const;
    BigInt operator+(int32_t rhs) const;

    BigInt operator-() const;
    BigInt operator-(const BigInt& rhs) const;
    BigInt operator-(int32_t rhs) const;

    BigInt operator*(const BigInt& rhs) const;
    BigInt operator*(int32_t rhs) const;

    bool operator==(const BigInt& rhs) const;
    bool operator!=(const BigInt& rhs) const;
    bool operator<(const BigInt& rhs) const;
    bool operator<=(const BigInt& rhs) const;
    bool operator>(const BigInt& rhs) const;
    bool operator>=(const BigInt& rhs) const;

    friend std::ostream& operator<<(std::ostream& out, const BigInt& bigint);

    ~BigInt();

private:
    void allocate(size_t newSize);
    void deallocate();
    void copy(const BigInt& rhs);

    int compareAbs(const BigInt& rhs) const;
    bool isZero() const;

private:
    char* digit_;
    size_t size_;
    bool isNegative_;
};
