#pragma once

#include <iostream>
#include <iomanip>
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

template<typename TDuration>
std::string format_time(TDuration duration, size_t rounds) {
    std::stringstream s;

    s << "\nTime elapsed: " << std::setfill('0') << std::setw(2)
            << duration / 1000 / 60 / 60 << ':' << std::setfill('0') << std::setw(2)
            << duration / 1000 / 60 % 60 << ':' << std::setfill('0') << std::setw(2)
            << duration / 1000 % 60 << '.'
            << duration % 1000 << '\n';

    auto each = duration / static_cast<long long>(rounds);
    s << "For each of the " << rounds << " rounds it averages to " << std::setfill('0') << std::setw(2)
            << each / 1000 / 60 / 60 << ':' << std::setfill('0') << std::setw(2)
            << each / 1000 / 60 % 60 << ':' << std::setfill('0') << std::setw(2)
            << each / 1000 % 60 << '.'
            << each % 1000 << " per round\n";

    return s.str();
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
