#include "lab1/matrix_ops.hpp"

#include <cstddef>

#include <gtest/gtest.h>

namespace lab1 {
namespace {

TEST(MatrixOpsTest, CreateAllocatesZeroInitializedMatrix) {
    int** m = matrix_create(2, 3);

    ASSERT_NE(m, nullptr);
    for (std::size_t i = 0; i < 2; ++i) {
        for (std::size_t j = 0; j < 3; ++j) {
            EXPECT_EQ(m[i][j], 0);
        }
    }

    matrix_delete(m, 2);
}

TEST(MatrixOpsTest, CreateWithZeroColumnsProducesNullRows) {
    int** m = matrix_create(2, 0);

    ASSERT_NE(m, nullptr);
    EXPECT_EQ(m[0], nullptr);
    EXPECT_EQ(m[1], nullptr);

    matrix_delete(m, 2);
}

TEST(MatrixOpsTest, FillSetsAllElements) {
    int** m = matrix_create(2, 3);

    matrix_fill(m, 2, 3, 7);

    for (std::size_t i = 0; i < 2; ++i) {
        for (std::size_t j = 0; j < 3; ++j) {
            EXPECT_EQ(m[i][j], 7);
        }
    }

    matrix_delete(m, 2);
}

TEST(MatrixOpsTest, RowMaxComputesMaximumPerRow) {
    int** m = matrix_create(2, 3);
    const int values[2][3]{{3, 1, 4}, {5, 2, 0}};
    for (std::size_t i = 0; i < 2; ++i) {
        for (std::size_t j = 0; j < 3; ++j) {
            m[i][j] = values[i][j];
        }
    }

    int* row_max = matrix_row_max(m, 2, 3);

    ASSERT_NE(row_max, nullptr);
    EXPECT_EQ(row_max[0], 4);
    EXPECT_EQ(row_max[1], 5);

    delete[] row_max;
    matrix_delete(m, 2);
}

TEST(MatrixOpsTest, ColMinComputesMinimumPerColumn) {
    int** m = matrix_create(2, 3);
    const int values[2][3]{{3, 1, 4}, {5, 2, 0}};
    for (std::size_t i = 0; i < 2; ++i) {
        for (std::size_t j = 0; j < 3; ++j) {
            m[i][j] = values[i][j];
        }
    }

    int* col_min = matrix_col_min(m, 2, 3);

    ASSERT_NE(col_min, nullptr);
    EXPECT_EQ(col_min[0], 3);
    EXPECT_EQ(col_min[1], 1);
    EXPECT_EQ(col_min[2], 0);

    delete[] col_min;
    matrix_delete(m, 2);
}

TEST(MatrixOpsTest, RowMaxHandlesSingleCell) {
    int** m = matrix_create(1, 1);
    m[0][0] = 42;

    int* row_max = matrix_row_max(m, 1, 1);

    ASSERT_NE(row_max, nullptr);
    EXPECT_EQ(row_max[0], 42);

    delete[] row_max;
    matrix_delete(m, 1);
}

TEST(MatrixOpsTest, ColMinHandlesNegativeValues) {
    int** m = matrix_create(2, 2);
    m[0][0] = -5;
    m[0][1] = 3;
    m[1][0] = -1;
    m[1][1] = -8;

    int* col_min = matrix_col_min(m, 2, 2);

    ASSERT_NE(col_min, nullptr);
    EXPECT_EQ(col_min[0], -5);
    EXPECT_EQ(col_min[1], -8);

    delete[] col_min;
    matrix_delete(m, 2);
}

TEST(MatrixOpsTest, DeleteDoesNotCrashOnNullptr) {
    matrix_delete(nullptr, 0);
}

TEST(MatrixOpsTest, PrintDoesNotCrashOnEmptyMatrix) {
    matrix_print(nullptr, 0, 0);
}

TEST(MatrixOpsTest_Fail, CreateWithZeroRowsReturnsNullptr) {
    EXPECT_EQ(matrix_create(0, 5), nullptr);
}

TEST(MatrixOpsTest_Fail, RowMaxWithZeroRowsReturnsNullptr) {
    EXPECT_EQ(matrix_row_max(nullptr, 0, 3), nullptr);
}

TEST(MatrixOpsTest_Fail, ColMinWithZeroColumnsReturnsNullptr) {
    int** m = matrix_create(2, 0);

    EXPECT_EQ(matrix_col_min(m, 2, 0), nullptr);

    matrix_delete(m, 2);
}

}  // namespace
}  // namespace lab1
