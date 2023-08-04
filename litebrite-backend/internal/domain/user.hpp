//
// Created by broosegoose on 7/9/23.
//

#ifndef DOMAIN_USER_HPP
#define DOMAIN_USER_HPP

#include <string>
#include <nlohmann/json.hpp>

namespace domain {

    struct User;
    typedef std::unique_ptr<User> UserPtr;

    struct User {

        int id;
        std::string email;
        std::string name;
        bool is_admin = false;
        int site_id = -1;

        [[nodiscard]] nlohmann::json to_json() const {
            /* don't include any sensitive fields */
            nlohmann::json j;
            j["id"] = id;
            j["email"] = email;
            j["name"] = name;
            j["is_admin"] = is_admin;
            j["site_id"] = site_id;
            return j;
        }

        static User from_json(const nlohmann::json& j) {
            /* allow creation of user with just email / password / site_id */
            return {
                .id = j.value("id", -1),
                .email = j.at("email"),
                .name = j.value("name", ""),
                .is_admin = j.value("is_admin", false),
                .site_id = j.at("site_id"),
            };
        }

    };
}

#endif //DOMAIN_USER_HPP
