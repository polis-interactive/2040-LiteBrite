//
// Created by broosegoose on 7/18/23.
//

#ifndef DOMAIN_INSTALLATION_HPP
#define DOMAIN_INSTALLATION_HPP

#include <cassert>
#include <vector>
#include <map>
#include <string>
#include <nlohmann/json.hpp>

#include "color.hpp"

namespace domain {

    struct Universe {
        unsigned int pixel_count;
        unsigned int pixel_width;
        unsigned int start_index;

        [[nodiscard]] inline unsigned int GetLastLength() const {
            return start_index + pixel_width * pixel_count;
        }

        [[nodiscard]] nlohmann::json to_json() const {
            nlohmann::json j;
            j["pixel_count"] = pixel_count;
            j["pixel_width"] = pixel_width;
            j["start_index"] = start_index;
            return j;
        }

        static Universe from_json(const nlohmann::json& j) {
            return {
                .pixel_count = j.at("pixel_count"),
                .pixel_width = j.at("pixel_width"),
                .start_index = j.at("start_index")
            };
        }
    };

    typedef std::map<unsigned int, Universe> UniverseMap;

    namespace installation {

        struct Layout {
            UniverseMap universes;
            std::map<std::string, std::vector<unsigned int>> controllers;

            [[nodiscard]] nlohmann::json to_json() const {
                nlohmann::json j;
                j["controllers"] = controllers;
                for (const auto &[key, value]: universes) {
                    j["universes"][std::to_string(key)] = value.to_json();
                }
                return j;
            }

            static Layout from_json(const nlohmann::json& j) {
                Layout l;
                l.controllers = j.at("controllers").get<std::map<std::string, std::vector<unsigned int>>>();
                for (const auto& [key, value]: j["universes"].items()) {
                    l.universes[std::stoi(key)] = Universe::from_json(value);
                }
                return l;
            }

        };

        struct Config {

            /* these should really be at the universe level / pixel level but w.e */
            bool rgbw_pixels;
            // only relevant to rgbw pixels
            std::optional<unsigned int> color_temperature;
            std::optional<CRGB> white_color;
            // available to all
            std::optional<CRGB> color_correction;
            std::optional<float> gamma;


            [[nodiscard]] nlohmann::json to_json() const {
                nlohmann::json j;
                j["rgbw_pixels"] = rgbw_pixels;
                if (color_temperature.has_value()) {
                    j["color_temperature"] = color_temperature.value();
                }
                if (white_color.has_value()) {
                    j["white_color"] = white_color.value().to_json();
                }
                if (color_correction.has_value()) {
                    j["color_correction"] = color_correction.value().to_json();
                }
                if (gamma.has_value()) {
                    j["gamma"] = gamma.value();
                }
                return j;
            }

            static Config from_json(const nlohmann::json& j) {
                Config c;
                c.rgbw_pixels = j.at("rgbw_pixels").get<bool>();
                if (j.contains("color_temperature")) {
                    c.color_temperature = { j.at("color_temperature").get<unsigned int>() };
                }
                if (j.contains("white_color")) {
                    c.white_color = { CRGB::from_json(j.at("white_color")) };
                }
                if (j.contains("color_correction")) {
                    c.color_correction = { CRGB::from_json(j.at("color_correction")) };
                }
                if (j.contains("gamma")) {
                    c.gamma = { j.at("gamma").get<float>() };
                }
                return c;
            }
        };

    }

}

#endif //DOMAIN_INSTALLATION_HPP
