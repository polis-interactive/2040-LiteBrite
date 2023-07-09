//
// Created by broosegoose on 7/9/23.
//

#ifndef DOMAIN_USER_HPP
#define DOMAIN_USER_HPP

#include <string>

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
    };

}

#endif //DOMAIN_USER_HPP
