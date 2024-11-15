#pragma once
#include <ostream>
#include <string>

class BigInt {

public:

    BigInt();
    BigInt(std::string input);
    BigInt(int32_t input);

    std::string& getNumber() const;
    BigInt& operator+(const BigInt&) const;
    void operator=(const BigInt&);
    friend std::ostream& operator<<(std::ostream& out, std::string number);

    ~BigInt();

private:
    char getNumber(size_t position) const;

private:
    std::string *number = 0;
};
