//
// Created by broosegoose on 7/22/23.
//

#ifndef DOMAIN_COLOR_HPP
#define DOMAIN_COLOR_HPP

#include <cstring>
#include <nlohmann/json.hpp>

namespace domain {

    struct CRGB {
        union {
            struct {
                union {
                    uint8_t r;
                };
                union {
                    uint8_t g;
                };
                union {
                    uint8_t b;
                };
            };
            uint8_t raw[3];
        };

        static CRGB FromColorTemperature(unsigned int kelvin) {
            // https://gist.github.com/byValle/b81391dcb2edaef9c651132ab3f123a9
            CRGB c = { 0 };
            float kelvin_f = kelvin / 100;
            // R
            if (kelvin_f <= 66) {
                c.r = 255;
            } else {
                const auto r_t = 329.698727446 * std::pow(kelvin_f - 60, -0.1332047592);
                c.r = (uint8_t) std::max(std::min(255.0, r_t), 0.0);
            }
            // G
            double g_t = 0.0;
            if (kelvin_f <= 66) {
                g_t = 99.4708025861 * std::log(kelvin_f) - 161.1195681661;
            } else {
                g_t = 288.1221695283 * std::pow(kelvin_f - 60, -0.0755148492);
            }
            c.g = (uint8_t) std::max(std::min(255.0, g_t), 0.0);
            // B
            if (kelvin_f <= 19) {
                c.b = 0;
            } else if (kelvin_f >= 66) {
                c.b = 255;
            } else {
                const auto b_t = 138.5177312231 * std::log(kelvin_f - 10) - 305.0447927307;
                c.b = (uint8_t) std::max(std::min(255.0, b_t), 0.0);
            }
            return c;
        }

        [[nodiscard]] nlohmann::json to_json() const {
            nlohmann::json j;
            j["r"] = r;
            j["g"] = g;
            j["b"] = b;
            return j;
        }

        static CRGB from_json(const nlohmann::json& j) {
            return {
                .r = j.at("r"),
                .g = j.at("g"),
                .b = j.at("b")
            };
        }
    };

    struct CRGBW {
        union {
            struct {
                union {
                    uint8_t r;
                };
                union {
                    uint8_t g;
                };
                union {
                    uint8_t b;
                };
                union {
                    uint8_t w;
                };
            };
            uint8_t raw[4];
        };

        [[nodiscard]] nlohmann::json to_json() const {
            nlohmann::json j;
            j["r"] = r;
            j["g"] = g;
            j["b"] = b;
            j["w"] = w;
            return j;
        }

        static CRGBW from_json(const nlohmann::json& j) {
            return {
                .r = j.at("r"),
                .g = j.at("g"),
                .b = j.at("b"),
                .w = j.at('w')
            };
        }
    };
}

#endif //DOMAIN_COLOR_HPP
