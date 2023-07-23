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
        double fps;
        DisplayType type;
        std::optional<domain::CRGB> rgb_color;
        std::optional<domain::CRGBW> rgbw_color;

        [[nodiscard]] std::string DisplayTypeToString() const {
            if (type == DisplayType::RGB) {
                return "RGB";
            } else if (type == DisplayType::RGBW) {
                return "RGBW";
            } else if (type == DisplayType::RGB_WITH_W_INTERPOLATION) {
                return "RGB_WITH_W_INTERPOLATION";
            }
        }

        [[nodiscard]] nlohmann::json to_json() const {
            nlohmann::json j;
            j["fps"] = fps;
            j["type"] = DisplayTypeToString();
            if (type == DisplayType::RGBW) {
                assert(rgbw_color.has_value());
                j["rgbw_color"] = rgbw_color.value().to_json();
            } else {
                assert(rgb_color.has_value());
                j["rgb_color"] = rgb_color.value().to_json();
            }
            return j;
        }

        static DisplayType DisplayTypeFromString(const std::string &display_type) {
            if (display_type == "RGB") {
                return DisplayType::RGB;
            } else if (display_type == "RGBW") {
                return DisplayType::RGBW;
            } else if (display_type == "RGB_WITH_W_INTERPOLATION") {
                return DisplayType::RGB_WITH_W_INTERPOLATION;
            } else {
                throw std::domain_error("Unknown domain::DisplayType: " + display_type);
            }
        }

        static Display from_json(const nlohmann::json& j) {
            Display d;
            d.fps = j.at("fps").get<double>();
            d.type = DisplayTypeFromString(j.at("type").get<std::string>());
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
