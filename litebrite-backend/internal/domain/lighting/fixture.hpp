//
// Created by brucegoose on 6/24/23.
//

#ifndef LITEBRITE_DOMAIN_LIGHTING_FIXTURE_HPP
#define LITEBRITE_DOMAIN_LIGHTING_FIXTURE_HPP

#include <set>
#include <vector>
#include <algorithm>
#include <map>

#include "pixel.hpp"

// fixture entity

// for now, lets only consider the fixtures we have; horizontal strips that are backwards

namespace domain {

    struct FixtureSummary {
        int max_y;
        std::map<uint8_t, unsigned int> universes;
    };

    class Fixture {
    public:
        Fixture(const uint8_t universe, const unsigned int start, const unsigned int length, const int x, const int y):
                _universe(universe), _start(start), _length(length), _x(x), _y(y)
        {}

        Fixture() = delete;
        Fixture (const Fixture&) = delete;
        Fixture& operator= (const Fixture&) = delete;

        void SummarizeFixture(FixtureSummary &summary) {
            if (auto universe = summary.universes.find(_universe); universe != summary.universes.end()) {
                universe->second += _length;
            } else {
                summary.universes.insert({_universe, _length});
            }
            summary.max_y = std::max(summary.max_y, _y);
        }
        std::vector<PixelPtr> GeneratePixels(){
            std::vector<PixelPtr> pixels;
            pixels.reserve(_length);
            std::generate_n(std::back_inserter(pixels), _length, [&]() {
                return std::make_shared<Pixel>(_length - _x - 1, _y);
            });
            return pixels;
        }
    private:
        const uint8_t _universe;
        const unsigned int _start;
        const unsigned int _length;
        const int _x;
        const int _y;
    };

}

#endif //LITEBRITE_DOMAIN_LIGHTING_FIXTURE_HPP
