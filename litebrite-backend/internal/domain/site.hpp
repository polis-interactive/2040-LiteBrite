//
// Created by broosegoose on 7/9/23.
//

#ifndef DOMAIN_SITE_HPP
#define DOMAIN_SITE_HPP

#include <string>
#include <nlohmann/json.hpp>

namespace domain {

    class Site {
        int id;
        std::string subdomain;
        std::string name;

        [[nodiscard]] nlohmann::json to_json() const {
            nlohmann::json j;
            j["id"] = id;
            j["subdomain"] = subdomain;
            j["name"] = name;
            return j;
        }

        void from_json(const nlohmann::json& j) {
            j.at("id").get_to(id);
            j.at("subdomain").get_to(subdomain);
            j.at("name").get_to(name);
        }

    };
}

#endif //DOMAIN_SITE_HPP
