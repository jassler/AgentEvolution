//
//  matrix.hpp
//  AgentEvolution
//
//  Created by Felix Fritz on 3/19/19.
//  Copyright © 2019 Felix Fritz. All rights reserved.
//

#ifndef matrix_hpp
#define matrix_hpp

#include <ostream>
#include <vector>
#include <initializer_list>
#include <sstream>

class Matrix {
private:
    double *m;
    size_t w, h;
    
    using iterator = double*;
    using const_iterator = double const*;
    
public:

    Matrix(size_t width, size_t height);
    Matrix(const std::initializer_list<std::initializer_list<double>>& matrix);
    
    template<typename Container>
    Matrix(const Container& matrix);
    
    Matrix(const Matrix& matrix);
    ~Matrix();
    
    size_t width() const;
    size_t height() const;
    size_t size() const;
    
    Matrix& operator=(const Matrix& a);
    bool operator==(const Matrix& a) const;
    double* operator[](const size_t& index) const;
    double* operator[](const int& index) const;
    
    iterator begin();
    iterator end();
    const_iterator begin() const;
    const_iterator end() const;
    
    friend Matrix operator*(const Matrix& a, const Matrix& b);
    friend std::vector<double> operator*(const Matrix& a, const std::vector<double>& v);
};

template<typename Container>
Matrix::Matrix(const Container& matrix) : w(matrix.begin()->size()), h(matrix.size()) {
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
};

#endif /* matrix_hpp */
