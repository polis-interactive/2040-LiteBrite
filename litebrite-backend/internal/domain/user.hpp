//
// Created by broosegoose on 7/9/23.
//

#ifndef DOMAIN_USER_HPP
#define DOMAIN_USER_HPP

#include <string>
#include <nlohmann/json.hpp>

namespace domain {

    struct User {
        int id;
        std::string email;
        /* salted and peppered password */
        std::string password;
        std::string salt;
        bool needs_password_change = true;
        bool is_admin = false;
        int site_id = -1;

        [[nodiscard]] nlohmann::json to_json() const {
            nlohmann::json j;
            j["id"] = id;
            j["email"] = email;
            j["password"] = password;
            j["salt"] = salt;
            j["needs_password_change"] = needs_password_change;
            j["is_admin"] = is_admin;
            j["site_id"] = site_id;
            return j;
        }

        void from_json(const nlohmann::json& j) {
            j.at("id").get_to(id);
            j.at("email").get_to(email);
            j.at("password").get_to(password);
            j.at("salt").get_to(salt);
            j.at("needs_password_change").get_to(needs_password_change);
            j.at("is_admin").get_to(is_admin);
            j.at("site_id").get_to(site_id);
        }
    };
}

#endif //DOMAIN_USER_HPP
