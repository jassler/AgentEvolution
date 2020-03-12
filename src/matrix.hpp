#ifndef MATRIX_HPP
#define MATRIX_HPP

#include "matrixexception.hpp"

#include <array>
#include <stdexcept>
#include <initializer_list>

// Matrix of type T with height H and width W
template<size_t H, size_t W, typename T=double>
class Matrix {
private:
    std::array<std::array<T, W>, H> m; 

public:
    // initialize as identity matrix
    Matrix();

    // copy constructor
    Matrix(const Matrix& other);

    // initialize inline
    Matrix(const std::initializer_list<std::initializer_list<T>>& matrix);

    constexpr auto& operator[](size_t i) noexcept;
    constexpr auto& operator[](size_t i) const noexcept;
    constexpr auto height() const noexcept;
    constexpr auto width() const noexcept;
    constexpr auto size() const noexcept;
    constexpr auto begin() noexcept;
    constexpr auto begin() const noexcept;
    constexpr auto end() noexcept;
    constexpr auto end() const noexcept;

    char *hash(size_t &len);

    // matrix multiplication
    // width of left matrix has to match height of right matrix
    // a[m,n] * b[n,o] -> c[m,o]
    template<size_t A_H, size_t AB, size_t B_W, typename _T>
    friend Matrix<A_H, B_W, _T> operator*(const Matrix<A_H, AB, _T>& a, const Matrix<AB, B_W, _T>& b);

    template<size_t _H, size_t _W, typename _T>
    friend std::array<_T, _H> operator*(const Matrix<_H, _W, _T>& m, const std::array<_T, _W>& v);

    // a == b
    template<size_t _H, size_t _W, typename _T>
    friend inline bool
    operator==(const Matrix<_H, _W, _T>& m1, const Matrix<_H, _W, _T>& m2);

    // a != b
    template<size_t _H, size_t _W, typename _T>
    friend inline bool
    operator!=(const Matrix<_H, _W, _T>& m1, const Matrix<_H, _W, _T>& m2);

    // assignment
    Matrix& operator=(const Matrix& b);

    // << to output stream
    template<size_t _H, size_t _W, typename _T>
    friend std::ostream&
    operator<<(std::ostream& os, Matrix<_H, _W, _T> m);
};

/*
 * MATRIX
 */
// initialize as identity matrix
template<size_t H, size_t W, typename T>
Matrix<H,W,T>::Matrix() {
    // everything 0
    for(auto& row : m)
        row.fill(0);
    
    if constexpr(H == W) {
        // diagonal 1
        for(size_t i = 0; i < W; ++i)
            m[i][i] = 1;
    } else if(H > W) {
        // ehm... do something
        double fill_with = 1.0 / static_cast<double>(std::max(W, H)) * static_cast<double>(std::min(W, H));
        for (size_t y = 0; y < H; ++y) {
            size_t x = y * W / H;
            m[y][x] = fill_with;
        }
    } else {
        // ehm... do the other thing
        double fill_with = 1.0 / static_cast<double>(std::max(W, H)) * static_cast<double>(std::min(W, H));
        for (size_t x = 0; x < W; ++x) {
            size_t y = x * H / W;
            m[y][x] = fill_with;
        }
    }
}

// copy constructor
template<size_t H, size_t W, typename T>
Matrix<H,W,T>::Matrix(const Matrix& other) {
    m = other.m;
}

// initialize through initializer list
// MatrixException gets thrown, if dimension of list is wrong
template<size_t H, size_t W, typename T>
Matrix<H,W,T>::Matrix(const std::initializer_list<std::initializer_list<T>>& matrix) {
    if(matrix.size() != H)
        throw MatrixException("Initializer list has wrong height (got ",
                            matrix.size(), ", expected ", H, ")");
    
    size_t y = 0;
    for(const auto& row : matrix) {
        if(row.size() != W)
            throw MatrixException("Initializer list has wrong width at row-index ",
                                y, " (got ", row.size(), ", expected ", W, ")");
        
        size_t x = 0;
        for(auto item : row) {
            m[y][x] = item;
            ++x;
        }
        ++y;
    }
}

// row index
template<size_t H, size_t W, typename T>
constexpr auto& Matrix<H,W,T>::operator[](size_t i) noexcept { return m[i]; }

// const row index
template<size_t H, size_t W, typename T>
constexpr auto& Matrix<H,W,T>::operator[](size_t i) const noexcept { return m[i]; }

// H
template<size_t H, size_t W, typename T>
constexpr auto Matrix<H,W,T>::height() const noexcept { return H; }

// W
template<size_t H, size_t W, typename T>
constexpr auto Matrix<H,W,T>::width() const noexcept { return W; }

// size == H * W
template<size_t H, size_t W, typename T>
constexpr auto Matrix<H,W,T>::size() const noexcept { return H * W; }

// begin
template<size_t H, size_t W, typename T>
constexpr auto Matrix<H,W,T>::begin() noexcept { return m.begin(); }

// begin const
template<size_t H, size_t W, typename T>
constexpr auto Matrix<H,W,T>::begin() const noexcept { return m.begin(); }

// end
template<size_t H, size_t W, typename T>
constexpr auto Matrix<H,W,T>::end() noexcept { return m.end(); }

// end const
template<size_t H, size_t W, typename T>
constexpr auto Matrix<H,W,T>::end() const noexcept { return m.end(); }

// convert matrix to char array (hashing)
template<size_t H, size_t W, typename T>
char *Matrix<H,W,T>::hash(size_t& len) {
    static char current_hash[H * W * sizeof(T)];
    T *ptr = reinterpret_cast<T *>(current_hash);
    len = H * W * sizeof(T);

    for(auto& row : m)
        for(auto col : row)
            *(ptr++) = col;

    return current_hash;
}

// matrix multiplication
// width of left matrix has to match height of right matrix
// a[m,n] * b[n,o] -> c[m,o]
template<size_t A_H, size_t AB, size_t B_W, typename T>
Matrix<A_H, B_W, T> operator*(const Matrix<A_H, AB, T>& a, const Matrix<AB, B_W, T>& b) {
    
    Matrix<A_H, B_W, T> result;
    for(size_t y = 0; y < A_H; ++y) {
        for(size_t x = 0; x < B_W; ++x) {
            
            double sum = 0;
            for(size_t i = 0; i < AB; ++i) {
                sum += a.m[y][i] * b.m[i][x];
            }
            result.m[y][x] = sum;
        }
    }
    
    return result;
}

template<size_t H, size_t W, typename T>
std::array<T, H> operator*(const Matrix<H, W, T>& m, const std::array<T, W>& v) {
    
    std::array<T, H> result;
    for(size_t y = 0; y < H; ++y) {
        double sum = 0;
        for(size_t x = 0; x < W; ++x) {
            sum += m[y][x] * v[x];
        }
        result[y] = sum;
    }

    return result;
}

// a == b
template<size_t H, size_t W, typename T>
inline bool
operator==(const Matrix<H, W, T>& m1, const Matrix<H, W, T>& m2) {
    return m1.m == m2.m;
}

// a != b
template<size_t H, size_t W, typename T>
inline bool
operator!=(const Matrix<H, W, T>& m1, const Matrix<H, W, T>& m2) {
    return m1.m != m2.m;
}

// assignment
template<size_t H, size_t W, typename T>
Matrix<H,W,T>& Matrix<H,W,T>::operator=(const Matrix& b) {
    if(&b != this)
        this->m = b.m;
    return *this;
}

// hacky matrix printer
template<size_t H, size_t W, typename T>
std::ostream& operator<<(std::ostream& os, Matrix<H, W, T> m) {
    os << "{";
    for(size_t y = 0; y < H; ++y) {
        os << "{";
        if constexpr(W > 0)
            os << m[y][0];
        for(size_t x = 1; x < W; ++x)
            os << "," << m[y][x];
        os << "}";
        if(y < H - 1)
            os << ",";
    }
    os << "}";
    return os;
}

#endif
