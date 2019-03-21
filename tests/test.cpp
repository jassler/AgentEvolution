#include "../AgentEvolution/matrix.hpp"
#include <iostream>
#include <cassert>

void test_matrix_init_1() {
    Matrix m(3, 3);

    m[0][0] = 1;
    m[0][1] = 4;
    m[0][2] = 9;
    m[1][0] = 5;
    m[1][1] = -4;
    m[1][2] = 0;
    m[2][0] = 1;
    m[2][1] = 4;
    m[2][2] = 7;

    assert(m[0][0] == 1);
    assert(m[0][1] == 4);
    assert(m[0][2] == 9);
    assert(m[1][0] == 5);
    assert(m[1][1] == -4);
    assert(m[1][2] == 0);
    assert(m[2][0] == 1);
    assert(m[2][1] == 4);
    assert(m[2][2] == 7);
}

void test_matrix_init_2() {
    Matrix m({
        { 1, 4, 9 },
        { 5, -4, 0 },
        { 1, 4, 7 }
    });

    assert(m[0][0] == 1);
    assert(m[0][1] == 4);
    assert(m[0][2] == 9);
    assert(m[1][0] == 5);
    assert(m[1][1] == -4);
    assert(m[1][2] == 0);
    assert(m[2][0] == 1);
    assert(m[2][1] == 4);
    assert(m[2][2] == 7);
}

void test_matrix_equal() {
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

    assert(m1 == m2);

    m2[0][0] = 0;
    assert(!(m1 == m2));
}

void test_matrix_multiplication() {
    Matrix v({
        { 1 },
        { 1 },
        { 1 }
    });

    Matrix m({
        { 1, 4, 9 },
        { 5, -4, 0 },
        { 1, 4, 7 }
    });

    Matrix res = m * v;
    std::cout << m[0][0] << " + " << m[0][1] << " + " << m[0][2] << " = " << res[0][0] << "\n";
    std::cout << "width: " << res.width << ", height: " << res.height << "\n";
    std::cout << res[0][1] << ", " << res[0][2] << "\n";
    assert(res[0][0] == m[0][0] + m[0][1] + m[0][2]);
    assert(res[0][1] == m[1][0] + m[1][1] + m[1][2]);
    assert(res[0][2] == m[2][0] + m[2][1] + m[2][2]);

    Matrix m2({
        { 2, 8 },
        { 10, 0 },
        { -1, 1 }
    });

    Matrix res2 = m * m2;
    assert(res2.height == 3);
    assert(res2.width == 2);

    Matrix expected({
        { 33, 17 },
        { -30, 40 },
        { 35, 15 }
    });

    assert(res2 == expected);
}

int main() {
    test_matrix_init_1();
    test_matrix_init_2();

    test_matrix_equal();
    test_matrix_multiplication();

    std::cout << "Tests passed\n";
}