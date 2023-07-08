//
// Created by brucegoose on 6/24/23.
//

#ifndef LITEBRITE_LIGHTING_INSTALLATION_HPP
#define LITEBRITE_LIGHTING_INSTALLATION_HPP

#include <map>

#include "fixture.hpp"

namespace domain {

    class Installation {
    public:
        Installation(const std::vector<Fixture> fixtures);
        Installation() = delete;
        Installation (const Installation&) = delete;
        Installation& operator= (const Installation&) = delete;
    private:
        std::vector<PixelPtr> _pixels;
        std::map<uint8_t, std::vector<PixelPtr>> _universes;
        std::vector<std::vector<PixelPtr>> _xy_grid;
    };

}


#endif //LITEBRITE_LIGHTING_INSTALLATION_HPP
