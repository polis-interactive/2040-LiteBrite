//
// Created by brucegoose on 6/24/23.
//

#ifndef LITEBRITE_DOMAIN_COLOR_HPP
#define LITEBRITE_DOMAIN_COLOR_HPP

#include <cstdint>

namespace domain {

    struct Color {
        union {
            struct {
                union {
                    uint8_t r;    ///< Red channel value
                };
                union {
                    uint8_t g;
                };
                union {
                    uint8_t b;     ///< Blue channel value
                };
            };
            uint8_t raw[3];
        };

        inline Color() __attribute__((always_inline)) = default;
    };

}

#endif //LITEBRITE_DOMAIN_COLOR_HPP
