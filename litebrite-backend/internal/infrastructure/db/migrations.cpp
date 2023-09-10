//
// Created by broosegoose on 7/10/23.
//

#include "db.hpp"
#include <iostream>

namespace infrastructure {


    void Db::clearDb() {
        /* needs to be up to date with migrations */
        // we can do this because of on delete cascade!
        const char* deleteSiteRows = "DELETE FROM sites;";
        _db->exec(deleteSiteRows);
    }

    std::vector<std::function<void(DatabasePtr &)>> Db::_migrations = {
        [](DatabasePtr &db) {
            db->exec(R"(
                CREATE TABLE IF NOT EXISTS sites (
                    id INTEGER PRIMARY KEY NOT NULL,
                    name TEXT NOT NULL,
                    slug TEXT NOT NULL
                );
            )");
            db->exec(R"(
                CREATE TABLE IF NOT EXISTS users (
                    id INTEGER PRIMARY KEY NOT NULL,
                    email TEXT NOT NULL,
                    name TEXT NOT NULL,
                    is_admin INTEGER NOT NULL,
                    site_id INTEGER NOT NULL,
                    FOREIGN KEY(site_id) REFERENCES sites(id) ON DELETE CASCADE
                );
            )");
        }
    };

}