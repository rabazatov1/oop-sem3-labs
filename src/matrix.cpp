#include "lab2/matrix.hpp"

#include <algorithm>
#include <cstddef>
#include <stdexcept>
#include <utility>

namespace lab2 {

namespace {

double* Allocate(std::size_t n) {
    return n == 0 ? nullptr : new double[n];
}

}  // namespace

Matrix::Matrix() : rows_{0}, cols_{0}, data_{nullptr} {}

Matrix::Matrix(std::size_t rows, std::size_t cols, double value)
    : rows_{rows}, cols_{cols}, data_{Allocate(rows * cols)} {
    std::fill(data_, data_ + rows_ * cols_, value);
}

Matrix::Matrix(const Matrix& other)
    : rows_{other.rows_}, cols_{other.cols_}, data_{Allocate(other.rows_ * other.cols_)} {
    std::copy(other.data_, other.data_ + rows_ * cols_, data_);
}

Matrix::Matrix(Matrix&& other) noexcept : rows_{other.rows_}, cols_{other.cols_}, data_{other.data_} {
    other.rows_ = 0;
    other.cols_ = 0;
    other.data_ = nullptr;
}

Matrix& Matrix::operator=(Matrix other) noexcept {
    swap(other);
    return *this;
}

Matrix::~Matrix() {
    delete[] data_;
}

void Matrix::swap(Matrix& other) noexcept {
    std::swap(rows_, other.rows_);
    std::swap(cols_, other.cols_);
    std::swap(data_, other.data_);
}

double Matrix::get(std::size_t r, std::size_t c) const {
    if (r >= rows_ || c >= cols_) {
        throw std::out_of_range("Matrix::get: index out of range");
    }
    return data_[r * cols_ + c];
}

void Matrix::set(std::size_t r, std::size_t c, double value) {
    if (r >= rows_ || c >= cols_) {
        throw std::out_of_range("Matrix::set: index out of range");
    }
    data_[r * cols_ + c] = value;
}

Matrix& Matrix::add(const Matrix& other) {
    if (rows_ != other.rows_ || cols_ != other.cols_) {
        throw std::invalid_argument("Matrix::add: dimension mismatch");
    }
    for (std::size_t i = 0; i < rows_ * cols_; ++i) {
        data_[i] += other.data_[i];
    }
    return *this;
}

Matrix& Matrix::mul(const Matrix& other) {
    if (cols_ != other.rows_) {
        throw std::invalid_argument("Matrix::mul: dimension mismatch");
    }

    Matrix result(rows_, other.cols_);
    for (std::size_t i = 0; i < rows_; ++i) {
        for (std::size_t k = 0; k < cols_; ++k) {
            const double a = data_[i * cols_ + k];
            for (std::size_t j = 0; j < other.cols_; ++j) {
                result.data_[i * other.cols_ + j] += a * other.data_[k * other.cols_ + j];
            }
        }
    }
    swap(result);
    return *this;
}

Matrix Matrix::submatrix(std::size_t r, std::size_t c) const {
    if (r >= rows_ || c >= cols_) {
        throw std::out_of_range("Matrix::submatrix: index out of range");
    }

    Matrix result(rows_ - 1, cols_ - 1);
    std::size_t out = 0;
    for (std::size_t i = 0; i < rows_; ++i) {
        if (i == r) {
            continue;
        }
        for (std::size_t j = 0; j < cols_; ++j) {
            if (j == c) {
                continue;
            }
            result.data_[out++] = data_[i * cols_ + j];
        }
    }
    return result;
}

void Matrix::print(std::ostream& os) const {
    os << '[';
    for (std::size_t i = 0; i < rows_; ++i) {
        if (i > 0) {
            os << ", ";
        }
        os << '[';
        for (std::size_t j = 0; j < cols_; ++j) {
            if (j > 0) {
                os << ", ";
            }
            os << data_[i * cols_ + j];
        }
        os << ']';
    }
    os << ']';
}

bool operator==(const Matrix& lhs, const Matrix& rhs) {
    return lhs.rows_ == rhs.rows_ && lhs.cols_ == rhs.cols_ &&
           std::equal(lhs.data_, lhs.data_ + lhs.rows_ * lhs.cols_, rhs.data_);
}

void swap(Matrix& a, Matrix& b) noexcept {
    a.swap(b);
}

std::ostream& operator<<(std::ostream& os, const Matrix& m) {
    m.print(os);
    return os;
}

}  // namespace lab2
