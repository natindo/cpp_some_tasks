#include <algorithm>
#include <cstdint>
#include <iostream>
#include <stdexcept>

class Matrix {
private:
    class ProxyRow {
    public:
        ProxyRow();
        ProxyRow(int32_t cols);
        ProxyRow(const ProxyRow& other);

        ProxyRow& operator=(const ProxyRow& other);
        ProxyRow& operator+(const ProxyRow& rhs);
        int32_t& operator[](int32_t j);
        void operator*=(int32_t i);
        bool operator==(const ProxyRow& rhs) const;
        
        ~ProxyRow();

    private:
        int32_t* data_ = nullptr;
        int32_t cols_number_ = 0;
    };

public:
    Matrix();
    Matrix(int32_t rows, int32_t cols);
    Matrix(const Matrix& other);
    Matrix& operator=(const Matrix& other);
    ~Matrix();

    int32_t getRows ();
    int32_t getCols ();
    ProxyRow& operator[](int32_t i);
    void operator*=(int32_t i);
    bool operator==(const Matrix& rhs) const;
    Matrix& operator+(const Matrix& rhs);
    friend std::ostream& operator<<(std::ostream& out, const Matrix& m);

private:
    int32_t cols_number_ = 0;
    int32_t rows_number_ = 0;
    ProxyRow *rows_ = nullptr;
};