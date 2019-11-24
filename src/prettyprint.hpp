#pragma once

#include <iostream>
#include <array>

template<typename T, size_t S>
std::ostream& operator<<(std::ostream& os, const std::array<T, S> arr) {
    if constexpr(S == 0) {
        os << "{ }";
    } else {
        os << "{ " << arr[0];
        for(size_t i = 1; i < S; ++i)
            os << ", " << arr[i];
        os << " }";
    }

    return os;
}
