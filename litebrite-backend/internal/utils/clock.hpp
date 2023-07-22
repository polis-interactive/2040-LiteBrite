//
// Created by broosegoose on 7/9/23.
//

#ifndef UTILS_CLOCK_HPP
#define UTILS_CLOCK_HPP

#include <chrono>
using namespace std::literals;

namespace utils {
    typedef std::chrono::duration<long int> Duration;
    typedef std::chrono::duration<double> QuickDuration;
    typedef std::chrono::high_resolution_clock Clock;
}

#endif //UTILS_CLOCK_HPP
