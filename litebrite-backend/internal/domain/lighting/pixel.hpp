//
// Created by brucegoose on 6/24/23.
//

#ifndef LITEBRITE_DOMAIN_LIGHTING_PIXEL_HPP
#define LITEBRITE_DOMAIN_LIGHTING_PIXEL_HPP

#include <cstdint>
#include <memory>

#include "domain/color/color.hpp"

// pixel value

namespace domain {

    class Pixel {
    public:
        Pixel(const int x, const int y):
                _x(x), _y(y)
        {}
        Pixel() = delete;
        Pixel (const Pixel&) = delete;
        Pixel& operator= (const Pixel&) = delete;

        void SetColor(Color &c) {
            _c = c;
        }
    private:
        const int _x;
        const int _y;
        Color _c;
    };

    typedef std::shared_ptr<Pixel> PixelPtr;

}

#endif //LITEBRITE_DOMAIN_LIGHTING_PIXEL_HPP
