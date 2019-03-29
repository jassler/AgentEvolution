#include "matrix.hpp"
#include "gtest/gtest.h"

TEST(MatrixTest, InitializationDefault) {
    Matrix m(3, 3);

    EXPECT_EQ(1, m[0][0]);
    EXPECT_EQ(1, m[1][1]);
    EXPECT_EQ(1, m[2][2]);

    EXPECT_EQ(0, m[0][1]);
    EXPECT_EQ(0, m[0][2]);
    EXPECT_EQ(0, m[1][0]);
    EXPECT_EQ(0, m[1][2]);
    EXPECT_EQ(0, m[2][0]);
    EXPECT_EQ(0, m[2][1]);

    m[0][0] = 1;
    m[0][1] = 4;
    m[0][2] = 9;
    m[1][0] = 5;
    m[1][1] = -4;
    m[1][2] = 0;
    m[2][0] = 1;
    m[2][1] = 4;
    m[2][2] = 7;

    EXPECT_EQ(1, m[0][0]);
    EXPECT_EQ(4, m[0][1]);
    EXPECT_EQ(9, m[0][2]);
    EXPECT_EQ(5, m[1][0]);
    EXPECT_EQ(-4, m[1][1]);
    EXPECT_EQ(0, m[1][2]);
    EXPECT_EQ(1, m[2][0]);
    EXPECT_EQ(4, m[2][1]);
    EXPECT_EQ(7, m[2][2]);
}

// initialize matrix with initializer list
TEST(MatrixTest, InitializationInline) {
    Matrix m({
        { 1, 4, 9 },
        { 5, -4, 0 },
        { 1, 4, 7 }
    });

    EXPECT_EQ(1, m[0][0]);
    EXPECT_EQ(4, m[0][1]);
    EXPECT_EQ(9, m[0][2]);
    EXPECT_EQ(5, m[1][0]);
    EXPECT_EQ(-4, m[1][1]);
    EXPECT_EQ(0, m[1][2]);
    EXPECT_EQ(1, m[2][0]);
    EXPECT_EQ(4, m[2][1]);
    EXPECT_EQ(7, m[2][2]);
}

TEST(MatrixTest, IsEqual) {
    Matrix m1(3, 3);
    m1[0][0] = 1;
    m1[0][1] = 4;
    m1[0][2] = 9;
    m1[1][0] = 5;
    m1[1][1] = -4;
    m1[1][2] = 0;
    m1[2][0] = 1;
    m1[2][1] = 4;
    m1[2][2] = 7;

    Matrix m2({
        { 1, 4, 9 },
        { 5, -4, 0 },
        { 1, 4, 7 }
    });

    Matrix m3({
        { 1, 4, 5, -4, 0, 1, 4, 7 }
    });

    EXPECT_TRUE(m1 == m2);

    m2[0][0] = 0;
    EXPECT_FALSE(m1 == m2);

    EXPECT_FALSE(m1 == m3);
}

TEST(MatrixTest, Multiplication) {
    Matrix v({
        { 1 },
        { 1 },
        { 1 }
    });

    Matrix m1({
        { 1, 4, 9 },
        { 5, -4, 0 },
        { 1, 4, 7 }
    });

    Matrix m2({
        { 2, 8 },
        { 10, 0 },
        { -1, 1 }
    });

    Matrix res1 = m1 * v;
    Matrix expected1({
        { 14 },
        { 1 },
        { 12 }
    });
    EXPECT_EQ(expected1.width(), res1.width());
    EXPECT_EQ(expected1.height(), res1.height());
    EXPECT_EQ(expected1, res1);

    Matrix res2 = m1 * m2;
    Matrix expected2({
        { 33, 17 },
        { -30, 40 },
        { 35, 15 }
    });

    EXPECT_EQ(expected2.width(), res2.width());
    EXPECT_EQ(expected2.height(), res2.height());
    EXPECT_EQ(expected2, res2);
}