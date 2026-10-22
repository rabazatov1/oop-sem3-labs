#include <cstddef>
#include <iostream>
#include <limits>
#include <stdexcept>

#include "lab2/matrix.hpp"

namespace {

void SkipBadInput() {
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

bool ReadDouble(double& out_value) {
    if (!(std::cin >> out_value)) {
        SkipBadInput();
        return false;
    }
    return true;
}

void RunCreate(lab2::Matrix& matrix) {
    std::cout << "Введите число строк, столбцов и значение для заполнения: ";
    std::size_t rows{};
    std::size_t cols{};
    double value{};
    if (!ReadSize(rows) || !ReadSize(cols) || !ReadDouble(value)) {
        std::cout << "Некорректные данные.\n";
        return;
    }

    matrix = lab2::Matrix(rows, cols, value);
    std::cout << "Матрица " << matrix.rows() << "x" << matrix.cols() << " создана.\n";
}

void RunSet(lab2::Matrix& matrix) {
    std::cout << "Введите строку, столбец и значение: ";
    std::size_t r{};
    std::size_t c{};
    double value{};
    if (!ReadSize(r) || !ReadSize(c) || !ReadDouble(value)) {
        std::cout << "Некорректные данные.\n";
        return;
    }

    try {
        matrix.set(r, c, value);
    } catch (const std::out_of_range& e) {
        std::cout << "Ошибка: " << e.what() << ".\n";
    }
}

void RunPrint(const lab2::Matrix& matrix) {
    matrix.print(std::cout);
    std::cout << '\n';
}

void RunSubmatrix(const lab2::Matrix& matrix) {
    std::cout << "Введите номер удаляемой строки и столбца: ";
    std::size_t r{};
    std::size_t c{};
    if (!ReadSize(r) || !ReadSize(c)) {
        std::cout << "Некорректные данные.\n";
        return;
    }

    try {
        std::cout << matrix.submatrix(r, c) << '\n';
    } catch (const std::out_of_range& e) {
        std::cout << "Ошибка: " << e.what() << ".\n";
    }
}

void PrintMenu() {
    std::cout
        << "\n1. Создать матрицу\n"
           "2. Задать элемент\n"
           "3. Напечатать\n"
           "4. Алгоритм варианта (submatrix)\n"
           "0. Выход\n"
           "Выбор: ";
}

}  // namespace

int main() {
    lab2::Matrix matrix;

    for (;;) {
        PrintMenu();
        int choice{};
        if (!ReadInt(choice)) {
            std::cout << "Некорректный пункт меню.\n";
            continue;
        }

        if (choice == 1) {
            RunCreate(matrix);
        } else if (choice == 2) {
            RunSet(matrix);
        } else if (choice == 3) {
            RunPrint(matrix);
        } else if (choice == 4) {
            RunSubmatrix(matrix);
        } else if (choice == 0) {
            break;
        } else {
            std::cout << "Неизвестный пункт меню.\n";
        }
    }

    return 0;
}
