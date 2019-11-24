#ifndef MATRIXEXCEPTION_HPP
#define MATRIXEXCEPTION_HPP

#include <sstream>
#include <stdexcept>

// Multiplication failed? Initialization errorprone?
// This class gets thrown!
class MatrixException : public std::exception {
private:
    const char* msg;

public:
    // Join arguments to exception message
    template<typename ... Args>
    MatrixException(Args ... args);

    const char* what() const noexcept { return this->msg; }
};

// Join arguments to exception message
template<typename ... Args>
MatrixException::MatrixException(Args ... args) {
    std::stringstream error_msg;
    (error_msg << ... << args);
    this->msg = error_msg.str().c_str();
}

#endif
