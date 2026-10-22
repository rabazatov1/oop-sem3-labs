#include "lab2/matrix.hpp"

#include <cstddef>
#include <initializer_list>
#include <sstream>
#include <stdexcept>
#include <utility>

#include <gtest/gtest.h>

namespace lab2 {
namespace {

Matrix Make(std::initializer_list<std::initializer_list<double>> rows) {
    const std::size_t r = rows.size();
    const std::size_t c = r == 0 ? 0 : rows.begin()->size();
    Matrix m(r, c);
    std::size_t i = 0;
    for (const auto& row : rows) {
        std::size_t j = 0;
        for (double v : row) {
            m.set(i, j, v);
            ++j;
        }
        ++i;
    }
    return m;
}

TEST(MatrixTest, DefaultConstructorCreatesEmptyMatrix) {
    const Matrix m;

    EXPECT_EQ(m.rows(), 0U);
    EXPECT_EQ(m.cols(), 0U);
}

TEST(MatrixTest, ConstructorFillsWithValue) {
    const Matrix m(2, 3, 1.5);

    EXPECT_EQ(m.rows(), 2U);
    EXPECT_EQ(m.cols(), 3U);
    for (std::size_t i = 0; i < 2; ++i) {
        for (std::size_t j = 0; j < 3; ++j) {
            EXPECT_DOUBLE_EQ(m.get(i, j), 1.5);
        }
    }
}

TEST(MatrixTest, ConstructorDefaultsToZero) {
    const Matrix m(2, 2);

    EXPECT_DOUBLE_EQ(m.get(1, 1), 0.0);
}

TEST(MatrixTest, SetAndGetUseRowMajorLayout) {
    Matrix m(2, 3);

    m.set(0, 2, 7.0);
    m.set(1, 0, 9.0);

    EXPECT_DOUBLE_EQ(m.get(0, 2), 7.0);
    EXPECT_DOUBLE_EQ(m.get(1, 0), 9.0);
    EXPECT_DOUBLE_EQ(m.get(0, 0), 0.0);
}

TEST(MatrixTest, CopyConstructorMakesDeepCopy) {
    Matrix a(2, 2, 1.0);

    Matrix b(a);
    b.set(0, 0, 5.0);

    EXPECT_DOUBLE_EQ(a.get(0, 0), 1.0);
    EXPECT_DOUBLE_EQ(b.get(0, 0), 5.0);
    EXPECT_EQ(b.rows(), 2U);
    EXPECT_EQ(b.cols(), 2U);
}

TEST(MatrixTest, CopyOfEmptyMatrixIsEmpty) {
    const Matrix a;

    const Matrix b(a);

    EXPECT_EQ(b.rows(), 0U);
    EXPECT_EQ(b.cols(), 0U);
}

TEST(MatrixTest, MoveConstructorStealsBufferAndEmptiesSource) {
    Matrix a = Make({{1, 2}, {3, 4}});

    Matrix b(std::move(a));

    EXPECT_EQ(b, Make({{1, 2}, {3, 4}}));
    EXPECT_EQ(a.rows(), 0U);
    EXPECT_EQ(a.cols(), 0U);
}

TEST(MatrixTest, CopyAssignmentMakesDeepCopy) {
    Matrix a(2, 2, 1.0);
    Matrix b(3, 1, 9.0);

    b = a;
    a.set(0, 0, 100.0);

    EXPECT_EQ(b.rows(), 2U);
    EXPECT_EQ(b.cols(), 2U);
    EXPECT_DOUBLE_EQ(b.get(0, 0), 1.0);
}

TEST(MatrixTest, MoveAssignmentTransfersContent) {
    Matrix a = Make({{1, 2}, {3, 4}});
    Matrix b(5, 5, 0.0);

    b = std::move(a);

    EXPECT_EQ(b, Make({{1, 2}, {3, 4}}));
    EXPECT_EQ(a.rows(), 0U);
}

TEST(MatrixTest, SelfAssignmentKeepsContent) {
    Matrix a = Make({{1, 2}, {3, 4}});
    Matrix& ref = a;

    a = ref;

    EXPECT_EQ(a, Make({{1, 2}, {3, 4}}));
}

TEST(MatrixTest, AddSumsElementwise) {
    Matrix a(2, 2, 1.0);
    Matrix b(2, 2, 2.0);

    a.add(b);

    EXPECT_EQ(a, Matrix(2, 2, 3.0));
    EXPECT_EQ(b, Matrix(2, 2, 2.0));
}

TEST(MatrixTest, MulSquareMatrices) {
    Matrix a(2, 2, 1.0);
    Matrix b(2, 2, 2.0);

    a.mul(b);

    EXPECT_EQ(a, Matrix(2, 2, 4.0));
}

TEST(MatrixTest, MulRectangularChangesShape) {
    Matrix a = Make({{1, 2, 3}, {4, 5, 6}});
    Matrix b = Make({{1}, {0}, {2}});

    a.mul(b);

    EXPECT_EQ(a, Make({{7}, {16}}));
}

TEST(MatrixTest, MulWithItselfIsSafe) {
    Matrix a = Make({{1, 2}, {3, 4}});

    a.mul(a);

    EXPECT_EQ(a, Make({{7, 10}, {15, 22}}));
}

TEST(MatrixTest, PrintFormatsRows) {
    std::ostringstream os{};

    Make({{1, 2}, {3, 4}}).print(os);

    EXPECT_EQ(os.str(), "[[1, 2], [3, 4]]");
}

TEST(MatrixTest, PrintEmptyMatrix) {
    std::ostringstream os{};

    Matrix().print(os);

    EXPECT_EQ(os.str(), "[]");
}

TEST(MatrixTest, SubmatrixRemovesMiddleRowAndColumn) {
    const Matrix m = Make({{1, 2, 3}, {4, 5, 6}, {7, 8, 9}});

    EXPECT_EQ(m.submatrix(1, 1), Make({{1, 3}, {7, 9}}));
}

TEST(MatrixTest, SubmatrixRemovesFirstRowAndColumn) {
    const Matrix m = Make({{1, 2, 3}, {4, 5, 6}, {7, 8, 9}});

    EXPECT_EQ(m.submatrix(0, 0), Make({{5, 6}, {8, 9}}));
}

TEST(MatrixTest, SubmatrixRemovesLastRowAndColumn) {
    const Matrix m = Make({{1, 2, 3}, {4, 5, 6}, {7, 8, 9}});

    EXPECT_EQ(m.submatrix(2, 2), Make({{1, 2}, {4, 5}}));
}

TEST(MatrixTest, SubmatrixOfRectangularMatrix) {
    const Matrix m = Make({{1, 2, 3, 4}, {5, 6, 7, 8}});

    const Matrix s = m.submatrix(0, 2);

    EXPECT_EQ(s.rows(), 1U);
    EXPECT_EQ(s.cols(), 3U);
    EXPECT_EQ(s, Make({{5, 6, 8}}));
}

TEST(MatrixTest, SubmatrixDoesNotModifyOriginal) {
    Matrix m = Make({{1, 2}, {3, 4}});

    Matrix s = m.submatrix(0, 0);
    s.set(0, 0, 100.0);

    EXPECT_EQ(m, Make({{1, 2}, {3, 4}}));
}

TEST(MatrixTest, SubmatrixOfOneElementMatrixIsEmpty) {
    const Matrix m(1, 1, 5.0);

    const Matrix s = m.submatrix(0, 0);

    EXPECT_EQ(s.rows(), 0U);
    EXPECT_EQ(s.cols(), 0U);
}

TEST(MatrixTest_Throw, GetAndSetOutOfRange) {
    Matrix m(2, 2);

    EXPECT_THROW(m.get(2, 0), std::out_of_range);
    EXPECT_THROW(m.get(0, 2), std::out_of_range);
    EXPECT_THROW(m.set(2, 2, 1.0), std::out_of_range);
    EXPECT_THROW(Matrix().get(0, 0), std::out_of_range);
}

TEST(MatrixTest_Throw, AddSizeMismatch) {
    Matrix a(2, 2);

    EXPECT_THROW(a.add(Matrix(2, 3)), std::invalid_argument);
    EXPECT_THROW(a.add(Matrix()), std::invalid_argument);
}

TEST(MatrixTest_Throw, MulSizeMismatchKeepsMatrix) {
    Matrix a = Make({{1, 2}, {3, 4}});

    EXPECT_THROW(a.mul(Matrix(3, 1)), std::invalid_argument);
    EXPECT_EQ(a, Make({{1, 2}, {3, 4}}));
}

TEST(MatrixTest_Throw, SubmatrixOutOfRange) {
    const Matrix m(3, 3, 1.0);

    EXPECT_THROW(m.submatrix(3, 0), std::out_of_range);
    EXPECT_THROW(m.submatrix(0, 3), std::out_of_range);
    EXPECT_THROW(Matrix().submatrix(0, 0), std::out_of_range);
}

}  // namespace
}  // namespace lab2
