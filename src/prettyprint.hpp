#pragma once

#include <iostream>
#include <sstream>
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

template<typename T>
std::ostream& operator<<(std::ostream& os, const std::initializer_list<T> list) {
    if(list.size() == 0) {
        os << "{ }";
    } else {
        auto it = list.begin();
        os << "{ " << *it;
        while(++it != list.end())
            os << ", " << *it ;
        os << " }";
    }
}

namespace pp {

    template<typename TContainer>
    std::string join(TContainer arr, char sep) {
        std::stringstream result;

        auto it = arr.begin();
        result << *it;
        while(++it != arr.end())
            result << sep << *it;

        return result.str();
    }
}
