#include <cstddef>
#include <iostream>
#include <limits>

#include "lab1/matrix_ops.hpp"

namespace {

constexpr std::size_t kMaxSize = 1000;

void SkipBadInput() {
    if (std::cin.eof()) {
        return;
    }
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

bool ReadSize(std::size_t& out_value) {
    long long raw{};
    if (!(std::cin >> raw) || raw < 0) {
        SkipBadInput();
        return false;
    }
    out_value = static_cast<std::size_t>(raw);
    return true;
}

bool ReadInt(int& out_value) {
    if (!(std::cin >> out_value)) {
        SkipBadInput();
        return false;
    }
    return true;
}

void PrintValues(const int* values, std::size_t size) {
    for (std::size_t i = 0; i < size; ++i) {
        std::cout << values[i] << ' ';
    }
    std::cout << '\n';
}

void RunCreate(int**& matrix, std::size_t& rows, std::size_t& cols) {
    std::cout << "Введите число строк и столбцов: ";
    std::size_t new_rows{};
    std::size_t new_cols{};
    if (!ReadSize(new_rows) || !ReadSize(new_cols)) {
        std::cout << "Некорректный размер.\n";
        return;
    }
    if (new_rows == 0 || new_rows > kMaxSize || new_cols > kMaxSize) {
        std::cout << "Строк должно быть от 1 до " << kMaxSize << ", столбцов от 0 до " << kMaxSize << ".\n";
        return;
    }

    lab1::matrix_delete(matrix, rows);
    matrix = lab1::matrix_create(new_rows, new_cols);
    rows = new_rows;
    cols = new_cols;
    std::cout << "Матрица " << rows << "x" << cols << " создана.\n";
}

void RunFill(int** matrix, std::size_t rows, std::size_t cols) {
    if (matrix == nullptr) {
        std::cout << "Матрица не создана.\n";
        return;
    }

    std::cout << "Введите значение для заполнения: ";
    int value{};
    if (!ReadInt(value)) {
        std::cout << "Некорректное значение.\n";
        return;
    }

    lab1::matrix_fill(matrix, rows, cols, value);
    std::cout << "Матрица заполнена значением " << value << ".\n";
}

void RunSetElement(int** matrix, std::size_t rows, std::size_t cols) {
    if (matrix == nullptr) {
        std::cout << "Матрица не создана.\n";
        return;
    }

    std::cout << "Введите строку, столбец (с нуля) и значение: ";
    std::size_t row{};
    std::size_t col{};
    int value{};
    if (!ReadSize(row) || !ReadSize(col) || !ReadInt(value)) {
        std::cout << "Некорректный ввод.\n";
        return;
    }
    if (row >= rows || col >= cols) {
        std::cout << "Элемента нет: размер матрицы " << rows << "x" << cols << ", нумерация с нуля.\n";
        return;
    }

    matrix[row][col] = value;
    std::cout << "m[" << row << "][" << col << "] = " << value << ".\n";
}

void RunPrint(const int* const* matrix, std::size_t rows, std::size_t cols) {
    if (matrix == nullptr) {
        std::cout << "Матрица не создана.\n";
        return;
    }

    lab1::matrix_print(matrix, rows, cols);
}

void RunAlgorithm(const int* const* matrix, std::size_t rows, std::size_t cols) {
    if (matrix == nullptr) {
        std::cout << "Матрица не создана.\n";
        return;
    }

    int* row_max = lab1::matrix_row_max(matrix, rows, cols);
    int* col_min = lab1::matrix_col_min(matrix, rows, cols);

    std::cout << "Максимумы строк: ";
    if (row_max == nullptr) {
        std::cout << "нет данных\n";
    } else {
        PrintValues(row_max, rows);
    }

    std::cout << "Минимумы столбцов: ";
    if (col_min == nullptr) {
        std::cout << "нет данных\n";
    } else {
        PrintValues(col_min, cols);
    }

    delete[] row_max;
    delete[] col_min;
}

void PrintMenu() {
    std::cout
        << "\n1. Создать матрицу\n"
           "2. Заполнить значением\n"
           "3. Напечатать\n"
           "4. Алгоритм варианта\n"
           "5. Задать элемент\n"
           "0. Выход\n"
           "Выбор: ";
}

}  // namespace

int main() {
    int** matrix = nullptr;
    std::size_t rows{0};
    std::size_t cols{0};

    for (;;) {
        PrintMenu();
        int choice{};
        if (!ReadInt(choice)) {
            if (std::cin.eof()) {
                break;
            }
            std::cout << "Некорректный пункт меню.\n";
            continue;
        }

        if (choice == 1) {
            RunCreate(matrix, rows, cols);
        } else if (choice == 2) {
            RunFill(matrix, rows, cols);
        } else if (choice == 3) {
            RunPrint(matrix, rows, cols);
        } else if (choice == 4) {
            RunAlgorithm(matrix, rows, cols);
        } else if (choice == 5) {
            RunSetElement(matrix, rows, cols);
        } else if (choice == 0) {
            break;
        } else {
            std::cout << "Неизвестный пункт меню.\n";
        }
    }

    lab1::matrix_delete(matrix, rows);
    matrix = nullptr;

    return 0;
}
