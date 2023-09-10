//
// Created by broosegoose on 7/10/23.
//

#include "db.hpp"

namespace infrastructure {

    bool Db::CreateUser(const domain::User &user) {
        SQLite::Statement query(
            *_db, "INSERT INTO users (email, name, is_admin, site_id) VALUES (?, ?, ?, ?)"
        );
        query.bind(1, user.email);
        query.bind(2, user.name);
        query.bind(3, user.is_admin ? 1 : 0);
        query.bind(4, user.site_id);
        int result = query.exec();
        return result == 1;
    }

    domain::UserPtr  Db::GetUser(const std::string &user_email) {
        SQLite::Statement query(*_db, "SELECT * FROM users WHERE email = ?");
        query.bind(1, user_email);

        if (query.executeStep()) {
            auto user = std::make_unique<domain::User>();
            user->id = query.getColumn("id");
            user->email = query.getColumn("email").getString();
            user->name = query.getColumn("name").getString();
            user->is_admin = query.getColumn("is_admin").getInt() != 0;
            user->site_id = query.getColumn("site_id");
            return user;
        }

        return nullptr;
    }

    domain::UserPtr  Db::GetUser(const int user_id) {
        SQLite::Statement query(*_db, "SELECT * FROM users WHERE id = ?");
        query.bind(1, user_id);

        if (query.executeStep()) {
            auto user = std::make_unique<domain::User>();
            user->id = query.getColumn("id");
            user->email = query.getColumn("email").getString();
            user->name = query.getColumn("name").getString();
            user->is_admin = query.getColumn("is_admin").getInt() != 0;
            user->site_id = query.getColumn("site_id");
            return user;
        }

        return nullptr;
    }

}