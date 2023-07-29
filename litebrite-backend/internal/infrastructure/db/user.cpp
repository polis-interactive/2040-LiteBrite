//
// Created by broosegoose on 7/10/23.
//

#include "db.hpp"

namespace infrastructure {

    bool Db::CreateUser(const domain::User &user) {
        auto mutable_user = user;
        const auto didHashPassword = _manager->HashPassword(mutable_user);
        if (!didHashPassword) {
            throw std::runtime_error("Unable to hash password for user: " + user.name);
        }

        SQLite::Statement query(
            *_db, "INSERT INTO users (email, name, password, salt, is_admin, site_id) VALUES (?, ?, ?, ?, ?, ?)"
        );
        query.bind(1, user.email);
        query.bind(2, user.name);
        query.bind(3, mutable_user.password);
        query.bind(4, mutable_user.salt);
        query.bind(5, user.is_admin ? 1 : 0);
        query.bind(6, user.site_id);
        int result = query.exec();
        return result == 1;
    }

    std::unique_ptr<domain::User> Db::GetUser(const std::string &user_email) {
        SQLite::Statement query(*_db, "SELECT * FROM users WHERE email = ?");
        query.bind(1, user_email);

        if (query.executeStep()) {
            auto user = std::make_unique<domain::User>();
            user->id = query.getColumn("id");
            user->email = query.getColumn("email").getString();
            user->name = query.getColumn("name").getString();
            user->password = query.getColumn("password").getString();
            user->salt = query.getColumn("salt").getString();
            user->is_admin = query.getColumn("is_admin").getInt() != 0;
            user->site_id = query.getColumn("site_id");
            return user;
        }

        return nullptr;
    }

}