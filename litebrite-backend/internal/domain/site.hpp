//
// Created by broosegoose on 7/9/23.
//

#ifndef DOMAIN_SITE_HPP
#define DOMAIN_SITE_HPP

#include <string>
#include <nlohmann/json.hpp>

namespace domain {

    struct Site;
    typedef std::unique_ptr<Site> SitePtr;

    struct Site {

        int id = -1;
        std::string name;
        std::string slug;

        [[nodiscard]] nlohmann::json to_json() const {
            nlohmann::json j;
            j["id"] = id;
            j["name"] = name;
            j["slug"] = slug;
            return j;
        }

        static Site from_json(const nlohmann::json& j) {
            return {
                .id = j.value("id", -1),
                .name = j.at("name"),
                .slug = j.at("slug")
            };
        }

    };
}

namespace nlohmann {
    inline void to_json(json& j, const domain::Site& site) {
        j = site.to_json();
    }
}

#endif //DOMAIN_SITE_HPP
