#pragma once

#include <cstddef>
#include <ostream>

namespace lab2 {

class Matrix {
public:
    Matrix();

    Matrix(std::size_t rows, std::size_t cols, double value = 0.0);

    Matrix(const Matrix& other);
    Matrix(Matrix&& other) noexcept;
    Matrix& operator=(Matrix other) noexcept;
    ~Matrix();

    void swap(Matrix& other) noexcept;

    std::size_t rows() const {
        return rows_;
    }

    std::size_t cols() const {
        return cols_;
    }

    double get(std::size_t r, std::size_t c) const;
    void set(std::size_t r, std::size_t c, double value);

    Matrix& add(const Matrix& other);

    Matrix& mul(const Matrix& other);

    Matrix submatrix(std::size_t r, std::size_t c) const;

    void print(std::ostream& os) const;

    friend bool operator==(const Matrix& lhs, const Matrix& rhs);

private:
    std::size_t rows_;
    std::size_t cols_;
    double* data_;
};

void swap(Matrix& a, Matrix& b) noexcept;
std::ostream& operator<<(std::ostream& os, const Matrix& m);

}  // namespace lab2
