//
// Created by broosegoose on 7/9/23.
//

#ifndef INFRASTRUCTURE_DB_HPP
#define INFRASTRUCTURE_DB_HPP

#include <memory>
#include <filesystem>

#include "SQLiteCpp/SQLiteCpp.h"

namespace infrastructure {

    struct DbConfig {
        std::filesystem::path db_path;
        std::string db_name;
    };

    class Db: std::enable_shared_from_this<Db> {
    public:
        static std::shared_ptr<Db> Create(const DbConfig &conf);
        explicit Db(const DbConfig &conf);

        // gets return nullptr on failure
        [[nodiscard]] std::unique_ptr<domain::User> GetUser(const int user_id);
        [[nodiscard]] std::unique_ptr<domain::User> GetUserWithCredentials(const int user_id);
        [[nodiscard]] bool CreateUser(const domain::User &user);
        [[nodiscard]] bool UpdateUser(const domain::User &user);

        // no copy assignment, no empty assignment
        Db() = delete;
        Db (const Db&) = delete;
        Db& operator= (const Db&) = delete;
    private:
        bool initialize();
        const std::filesystem::path _db_path;
        const std::string _db_name;
        std::unique_ptr<SQLite::Database> _db = nullptr;
    };
}

#endif //INFRASTRUCTURE_DB_HPP
