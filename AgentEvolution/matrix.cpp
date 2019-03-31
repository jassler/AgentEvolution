//
//  matrix.cpp
//  AgentEvolution
//
//  Created by Felix Fritz on 3/19/19.
//  Copyright © 2019 Felix Fritz. All rights reserved.
//

#include "matrix.hpp"
#include <sstream>

Matrix::Matrix(size_t width, size_t height) : w(width), h(height) {
    m = new double[width * height];
    for(size_t y = 0; y < height; ++y) {
        for(size_t x = 0; x < width; ++x) {
            if(x == y)
                m[y * width + x] = 1;
            else
                m[y * width + x] = 0;
        }
    }
}

Matrix::Matrix(const std::initializer_list<std::initializer_list<double>>& matrix) : w(matrix.begin()->size()), h(matrix.size()) {
    m = new double[w * h];
    
    size_t index = 0;

    for(auto y : matrix) {
        if(y.size() != w) {
            std::stringstream ss;
            ss << "Expected matrix width to be " << w << ", instead got " << y.size() << " in row " << index / w;
            throw ss.str();
        }

        for(auto x : y) {
            m[index] = x;
            index++;
        }
    }
}

Matrix::Matrix(const Matrix& matrix) : w(matrix.w), h(matrix.h) {
    m = new double[w * h];
    for(size_t i = 0; i < w * h; ++i) {
        m[i] = matrix.m[i];
    }
}

Matrix::~Matrix() {
    delete[] m;
}

size_t Matrix::width() const {
    return w;
}

size_t Matrix::height() const {
    return h;
}

size_t Matrix::size() const {
    return w * h;
}

Matrix operator*(const Matrix& a, const Matrix& b) {
    if(a.w != b.h) {
        throw "width of left matrix != height of right matrix";
    }
    
    Matrix result(b.w, a.h);
    int count = 0;
    for(size_t y = 0; y < a.h; ++y) {
        for(size_t x = 0; x < b.w; ++x) {
            
            double sum = 0;
            for(size_t i = 0; i < a.w; ++i) {
                sum += a.m[y * a.w + i] * b.m[i * b.w + x];
            }
            result.m[count] = sum;
            ++count;
        }
    }
    
    return result;
}

std::vector<double> operator*(const Matrix& a, const std::vector<double>& v) {
    if(v.size() != a.width()) {
        throw "amount of elements in vector has to match matrix height, but matrix height is " + std::to_string(a.height()) + " and vector length is " + std::to_string(v.size());
    }
    
    std::vector<double> result(v.size());
    for(size_t y = 0; y < a.height(); ++y) {
        
        double sum = 0;
        for(size_t i = 0; i < a.width(); ++i) {
            sum += v[i] * a[y][i];
        }
        
        result[y] = sum;
    }
    
    return result;
}

Matrix& Matrix::operator=(const Matrix& a) {
    if(this == &a) {
        return *this;
    }
    
    if(a.w != w || a.h != h) {
        delete[] m;
        m = new double[a.w * a.h];
        w = a.w;
        h = a.h;
    }
    
    for(size_t i = 0; i < w * h; ++i) {
        m[i] = a.m[i];
    }
    
    return *this;
}

bool Matrix::operator==(const Matrix& a) const {
    if(w != a.w || h != a.h)
        return false;
    
    for(size_t i = 0; i < w * h; ++i) {
        if(m[i] != a.m[i])
            return false;
    }
    return true;
}

double* Matrix::operator[](const size_t& index) const {
    return &m[index * w];
}

std::ostream& operator<<(std::ostream& os, const Matrix& m) {
    
    os << "{\n";
    
    for(size_t y = 0; y < m.h; ++y) {
        os << "    { " << m.m[y * m.w];

        for(size_t x = 1; x < m.w; ++x) {
            os << ", " << m.m[y * m.w + x];
        }
        if(y < m.h - 1)
            os << " },\n";
        else
            os << " }\n";
    }

    os << '}';
    
    return os;
}

Matrix::iterator Matrix::begin() {
    return m;
}

Matrix::iterator Matrix::end() {
    return &m[w * h];
}

Matrix::const_iterator Matrix::begin() const {
    return m;
}

Matrix::const_iterator Matrix::end() const {
    return &m[w * h];
}
