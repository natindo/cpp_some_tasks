#include <cassert>
#include "matrix.h"

Matrix::ProxyRow::ProxyRow() :data_(nullptr), cols_number_(0){}

Matrix::ProxyRow::ProxyRow(int32_t cols) : cols_number_(cols) {
    data_ = new int32_t [cols];
}

Matrix::ProxyRow::ProxyRow(const ProxyRow& other) {
    data_ = new int32_t[cols_number_];
    std::copy(other.data_, other.data_ + cols_number_, data_);
}

int32_t& Matrix::ProxyRow::operator[](int32_t j) {
    if (j > cols_number_ || j < 0) {
        throw std::out_of_range("");
    }
    return data_[j];
}

void Matrix::ProxyRow::operator*=(int32_t i) {
    for(int32_t j = 0; j < cols_number_; ++j) {
        data_[j] *= i;
    }
}

Matrix::ProxyRow& Matrix::ProxyRow::operator=(const ProxyRow& other) {
    if(this != &other) {
        delete[] data_;
        cols_number_ = other.cols_number_;
        data_ = new int32_t[cols_number_];
        std::copy(other.data_, other.data_ + cols_number_, data_);
    }

    return *this;
}

bool Matrix::ProxyRow::operator==(const ProxyRow& rhs) const {
    for (int i = 0; i < cols_number_; ++i) {
        if (this->data_[i] != rhs.data_[i]) {
            return false;
        }
    }
    return true;
}

Matrix::ProxyRow& Matrix::ProxyRow::operator+(const ProxyRow& rhs) {
    for (int i = 0; i < cols_number_; ++i) {
        this->data_[i] += rhs.data_[i];
    }
    return *this;
}

Matrix::ProxyRow::~ProxyRow() {
    delete[] data_;
}

Matrix::Matrix() : cols_number_(0), rows_number_(0), rows_(nullptr){}

Matrix::Matrix(int32_t rows, int32_t cols) : cols_number_(cols), rows_number_(rows), rows_(nullptr) {
    if (rows > rows_number_ || rows < 0) {
        throw std::out_of_range("");
    }

    if (cols > cols_number_ || cols < 0) {
        throw std::out_of_range("");
    }

    rows_ = new ProxyRow[rows_number_];
    for (int32_t i = 0; i < rows_number_; ++i) {
        rows_[i] = ProxyRow(cols_number_);
    }

}

Matrix::Matrix(const Matrix& other) : cols_number_(other.cols_number_), rows_number_(other.rows_number_), rows_(nullptr) {
    if (rows_number_ > 0) {
        rows_ = new ProxyRow[rows_number_];
        for (int32_t i = 0; i < rows_number_; ++i) {
            rows_[i] = other.rows_[i];
        }
    }
}

Matrix& Matrix::operator=(const Matrix& other) {
    if (this != &other) {
        delete[] rows_;
        cols_number_ = other.cols_number_;
        rows_number_ = other.rows_number_;
        if (rows_number_ > 0) {
            rows_ = new ProxyRow[rows_number_];
            for (int32_t i = 0; i < rows_number_; ++i) {
                rows_[i] = other.rows_[i];
            }
        } else {
            rows_ = nullptr;
        }
    }
    return *this;
}

Matrix::~Matrix() {
    delete[] rows_;
}

int32_t Matrix::getRows () {
    return rows_number_;
}

int32_t Matrix::getCols () {
    return cols_number_;
}

Matrix::ProxyRow& Matrix::operator[](int32_t i) {
    if (i > rows_number_ || i < 0) {
        throw std::out_of_range("");
    }
    return rows_[i];
}

void Matrix::operator*=(int32_t i) {
    for(int32_t j = 0; j < rows_number_; ++j) {
        rows_[j] *= i;
    }
}

    bool Matrix::operator==(const Matrix& rhs) const {
        for (auto i = 0; i < rows_number_; ++i) {
            if (!(rows_[i] == rhs.rows_[i])) {
                return false;
            }
        }
        return true;
    }

Matrix& Matrix::operator+(const Matrix& rhs) {

    if (rows_number_ != rhs.rows_number_ || cols_number_ != rhs.cols_number_) {
        throw std::out_of_range("");
    }

    for (auto i = 0; i < rows_number_; ++i) {
        rows_[i] = rows_[i] + rhs.rows_[i];

    }
    return *this;
}

std::ostream& operator<<(std::ostream& out, const Matrix& m) {
    for (int32_t i = 0; i < m.rows_number_; ++i) {
        for (int32_t j = 0; j < m.cols_number_; ++j) {
            out << m.rows_[i][j] << "  ";
        }
        out << std::endl;
    }
    return out;
}
