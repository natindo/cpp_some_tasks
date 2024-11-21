#pragma once
#include <cstddef>
#include <ostream>
#include <string>

class BigInt {

public:

    BigInt();
    BigInt(int32_t input);
    BigInt(std::string input);
    BigInt(const BigInt& rhs);
    BigInt(BigInt&& rhs);

    BigInt operator=(const BigInt& rhs);
    //<> operator=(BigInt&& rhs);

    BigInt& operator+(const BigInt& rhs) const;
    //BigInt& operator+(BigInt&& rhs) const;

    const std::string& getNumber() const;
    
    friend std::ostream& operator<<(std::ostream& out, std::string number);

    ~BigInt();

private:
    char* digit;
    size_t size;

    void allocate(size_t newSize);
};
