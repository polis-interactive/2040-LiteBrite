//
// Created by broosegoose on 7/9/23.
//

#ifndef DOMAIN_SITE_HPP
#define DOMAIN_SITE_HPP

#include <string>
#include <nlohmann/json.hpp>

namespace domain {

    struct Site {

        int id = -1;
        std::string name;
        std::string subdomain;

        [[nodiscard]] nlohmann::json to_json() const {
            nlohmann::json j;
            j["id"] = id;
            j["name"] = name;
            j["subdomain"] = subdomain;
            return j;
        }

        static Site from_json(const nlohmann::json& j) {
            return {
                .id = j.value("id", -1),
                .name = j.at("name"),
                .subdomain = j.at("subdomain"),
            };
        }

    };
}

#endif //DOMAIN_SITE_HPP
