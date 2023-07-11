//
// Created by broosegoose on 7/10/23.
//

#include "db.hpp"

namespace infrastructure {

    bool Db::CreateUser(const domain::User &user) {
        SQLite::Statement query(
            *_db, "INSERT INTO users (email, password, salt, is_admin, site_id) VALUES (?, ?, ?, ?, ?)"
        );
        query.bind(1, user.email);
        query.bind(2, user.password);
        query.bind(3, user.salt);
        query.bind(4, user.is_admin ? 1 : 0);
        query.bind(5, user.site_id);
        int result = query.exec();  // Executes the insert operation
        return result == 1;
    }

}