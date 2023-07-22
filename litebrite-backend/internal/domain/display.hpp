//
// Created by broosegoose on 7/22/23.
//

#ifndef DOMAIN_DISPLAY_HPP
#define DOMAIN_DISPLAY_HPP

#include <nlohmann/json.hpp>

#include "color.hpp"

namespace domain {

    enum class DisplayType {
        RGB = 0,
        RGBW,
        RGB_WITH_W_INTERPOLATION
    };

    struct Display {
        unsigned int buffer_count;
        double fps;
        DisplayType type;
        std::optional<domain::CRGB> rgb_color;
        std::optional<domain::CRGBW> rgbw_color;

        [[nodiscard]] nlohmann::json to_json() const {
            nlohmann::json j;
            j["buffer_count"] = buffer_count;
            j["fps"] = fps;
            j["type"] = type;
            if (type == DisplayType::RGBW) {
                assert(rgbw_color.has_value());
                j["rgbw_color"] = rgbw_color.value().to_json();
            } else {
                assert(rgb_color.has_value());
                j["rgb_color"] = rgb_color.value().to_json();
            }
            return j;
        }

        static Display from_json(const nlohmann::json& j) {
            Display d;
            d.buffer_count = j.at("buffer_count").get<unsigned int>();
            d.fps = j.at("fps").get<double>();
            d.type = j.at("type").get<DisplayType>();
            if (d.type == DisplayType::RGBW) {
                assert(j.contains("rgbw_color"));
                d.rgbw_color = { CRGBW::from_json(j.at("rgbw_color")) };
            } else {
                assert(j.contains("rgb_color"));
                d.rgb_color = { CRGB::from_json(j.at("rgb_color")) };
            }
            return d;
        }
    };

}

#endif //DOMAIN_DISPLAY_HPP
