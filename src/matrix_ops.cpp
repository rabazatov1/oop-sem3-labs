#include "lab1/matrix_ops.hpp"

#include <cstddef>
#include <iostream>

namespace lab1 {

int** matrix_create(std::size_t rows, std::size_t cols) {
    if (rows == 0) {
        return nullptr;
    }

    int** m = new int*[rows];
    for (std::size_t i = 0; i < rows; ++i) {
        m[i] = cols == 0 ? nullptr : new int[cols]();
    }
    return m;
}

void matrix_delete(int** m, std::size_t rows) {
    if (m == nullptr) {
        return;
    }

    for (std::size_t i = 0; i < rows; ++i) {
        delete[] m[i];
    }
    delete[] m;
}

void matrix_fill(int** m, std::size_t rows, std::size_t cols, int value) {
    if (m == nullptr) {
        return;
    }

    for (std::size_t i = 0; i < rows; ++i) {
        for (std::size_t j = 0; j < cols; ++j) {
            m[i][j] = value;
        }
    }
}

void matrix_print(const int* const* m, std::size_t rows, std::size_t cols) {
    if (m == nullptr) {
        return;
    }

    for (std::size_t i = 0; i < rows; ++i) {
        for (std::size_t j = 0; j < cols; ++j) {
            std::cout << m[i][j] << ' ';
        }
        std::cout << '\n';
    }
}

int* matrix_row_max(const int* const* m, std::size_t rows, std::size_t cols) {
    if (m == nullptr || rows == 0 || cols == 0) {
        return nullptr;
    }

    int* result = new int[rows];
    for (std::size_t i = 0; i < rows; ++i) {
        int row_max = m[i][0];
        for (std::size_t j = 1; j < cols; ++j) {
            if (m[i][j] > row_max) {
                row_max = m[i][j];
            }
        }
        result[i] = row_max;
    }
    return result;
}

int* matrix_col_min(const int* const* m, std::size_t rows, std::size_t cols) {
    if (m == nullptr || rows == 0 || cols == 0) {
        return nullptr;
    }

    int* result = new int[cols];
    for (std::size_t j = 0; j < cols; ++j) {
        int col_min = m[0][j];
        for (std::size_t i = 1; i < rows; ++i) {
            if (m[i][j] < col_min) {
                col_min = m[i][j];
            }
        }
        result[j] = col_min;
    }
    return result;
}

}  // namespace lab1
