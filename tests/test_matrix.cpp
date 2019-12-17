#include "matrix.hpp"
#include "gtest/gtest.h"

TEST(MatrixTest, InitializationDefault) {
    Matrix<3, 3> m;

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
    Matrix<3, 3> m({
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

// Assignment operator
TEST(MatrixTest, Assignment) {
    Matrix<2, 3> m1;
    Matrix<2, 3> m2({
        { 2, 4, 6 },
        { -2, -6, -1 }
    });

    m1 = m2;

    EXPECT_EQ(m2, m1);
    EXPECT_EQ((Matrix<2,3>({{2,4,6},{-2,-6,-1}})), m1);
}

// MatrixException should be thrown here
TEST(MatrixTest, InitializationInlineException) {
    EXPECT_ANY_THROW((Matrix<2, 1>({{1}, {1,2}})));
    EXPECT_ANY_THROW((Matrix<2, 1>({{1}, {2}, {3}})));
    EXPECT_ANY_THROW((Matrix<1, 2>({{1,2}, {1}})));
}

TEST(MatrixTest, IsEqual) {
    Matrix<3, 3> m1;
    m1[0][0] = 1;
    m1[0][1] = 4;
    m1[0][2] = 9;
    m1[1][0] = 5;
    m1[1][1] = -4;
    m1[1][2] = 0;
    m1[2][0] = 1;
    m1[2][1] = 4;
    m1[2][2] = 7;

    Matrix<3, 3> m2({
        { 1, 4, 9 },
        { 5, -4, 0 },
        { 1, 4, 7 }
    });

    Matrix<3, 3> m3({
        { 1, 4, 5}, { -4, 0, 1}, { 4, 7, 0 } }
    );

    EXPECT_TRUE(m1 == m2);

    m2[0][0] = 0;
    EXPECT_FALSE(m1 == m2);

    EXPECT_FALSE(m1 == m3);
}

TEST(MatrixTest, MatrixMultiplication) {
    Matrix<3, 1> v({
        { 1 },
        { 1 },
        { 1 }
    });

    Matrix<3, 3> m1({
        { 1, 4, 9 },
        { 5, -4, 0 },
        { 1, 4, 7 }
    });

    Matrix<3, 2> m2({
        { 2, 8 },
        { 10, 0 },
        { -1, 1 }
    });

    Matrix res1 = m1 * v;
    Matrix<3, 1> expected1({
        { 14 },
        { 1 },
        { 12 }
    });
    EXPECT_EQ(expected1.width(), res1.width());
    EXPECT_EQ(expected1.height(), res1.height());
    EXPECT_EQ(expected1, res1);

    Matrix res2 = m1 * m2;
    Matrix<3, 2> expected2({
        { 33, 17 },
        { -30, 40 },
        { 35, 15 }
    });

    EXPECT_EQ(expected2.width(), res2.width());
    EXPECT_EQ(expected2.height(), res2.height());
    EXPECT_EQ(expected2, res2);
}

TEST(MatrixTest, ArrayMultiplication) {
    Matrix<2, 3> m({{2, 3, 4}, {-1, 1, -2}});
    std::array<double, 3> arr({3, 2, 5});
    std::array<double, 2> expected({32, -11});
    EXPECT_EQ(expected, (m * arr));
}

TEST(MatrixTest, Hash) {
    Matrix<1, 4, double> m1({{0.5, 4, -1.91, 22}});

    size_t size;
    char *hash = m1.hash(size);

    EXPECT_EQ(size, 4 * sizeof(double));

    double d = *(reinterpret_cast<double *>(hash));
    EXPECT_EQ(d, 0.5);

    hash += sizeof(double);
    d = *(reinterpret_cast<double *>(hash));
    EXPECT_EQ(d, 4);

    hash += sizeof(double);
    d = *(reinterpret_cast<double *>(hash));
    EXPECT_EQ(d, -1.91);

    hash += sizeof(double);
    d = *(reinterpret_cast<double *>(hash));
    EXPECT_EQ(d, 22);
}