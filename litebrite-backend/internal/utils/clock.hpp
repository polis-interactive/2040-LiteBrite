//
// Created by broosegoose on 7/9/23.
//

#ifndef UTILS_CLOCK_HPP
#define UTILS_CLOCK_HPP

#include <chrono>
using namespace std::literals;
typedef std::chrono::high_resolution_clock Clock;

namespace utils {
    typedef std::chrono::duration<long int> duration;
}

#endif //UTILS_CLOCK_HPP
