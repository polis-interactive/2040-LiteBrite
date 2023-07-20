//
// Created by broosegoose on 7/18/23.
//

#ifndef DOMAIN_INSTALLATION_HPP
#define DOMAIN_INSTALLATION_HPP

#include <vector>
#include <map>
#include <string>
#include <nlohmann/json.hpp>

namespace domain {

    struct Universe {
        unsigned int pixel_count;
        unsigned int pixel_width;
        unsigned int start_index;

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

    struct Installation {
        std::map<unsigned int, Universe> universes;
        std::map<std::string, std::vector<unsigned int>> controllers;

        [[nodiscard]] nlohmann::json to_json() const {
            nlohmann::json j;
            j["controllers"] = controllers;
            for (const auto &[key, value]: universes) {
                j["universes"][std::to_string(key)] = value.to_json();
            }
            return j;
        }

        static Installation from_json(const nlohmann::json& j) {
            Installation i;
            i.controllers = j.at("controllers").get<std::map<std::string, std::vector<unsigned int>>>();
            for (const auto& [key, value]: j["universes"].items()) {
                i.universes[std::stoi(key)] = Universe::from_json(value);
            }
            return i;
        }

    };

}

#endif //DOMAIN_INSTALLATION_HPP
