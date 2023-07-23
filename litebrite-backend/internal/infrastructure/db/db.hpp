//
// Created by broosegoose on 7/9/23.
//

#ifndef INFRASTRUCTURE_DB_HPP
#define INFRASTRUCTURE_DB_HPP

#include <memory>
#include <filesystem>
#include <vector>
#include <functional>

#include <SQLiteCpp/SQLiteCpp.h>

#include "domain/user.hpp"
#include "domain/site.hpp"


namespace infrastructure {


    typedef std::unique_ptr<SQLite::Database> DatabasePtr;

    struct DbConfig {
        std::filesystem::path db_path;
        std::string db_name;
        bool clear_db;
        bool seed_db;
    };

    class Db;
    typedef std::shared_ptr<Db> DbPtr;

    class Db: std::enable_shared_from_this<Db> {
    public:
        [[nodiscard]] static DbPtr Create(const DbConfig &conf);
        explicit Db(const DbConfig &conf);

        // public for test; and I'm not fancy enough to protect friend it with some test class...
        [[nodiscard]] int32_t hashSeedData(const nlohmann::json &seed);

        /* User CRUD */
        [[nodiscard]] bool CreateUser(const domain::User &user);
        [[nodiscard]] bool CreateOrUpdateUser(const domain::User &user);
        // gets return nullptr on failure
        [[nodiscard]] std::unique_ptr<domain::User> GetUser(const int user_id);
        [[nodiscard]] std::unique_ptr<domain::User> GetUserWithCredentials(const int user_id);
        [[nodiscard]] bool UpdateUser(const domain::User &user);
        [[nodiscard]] bool DeleteUser(const domain::User &user);
        [[nodiscard]] bool DeleteUser(const int user_id);

        /* Site CRUD */
        [[nodiscard]] bool CreateSite(const domain::Site &site);
        [[nodiscard]] bool CreateOrUpdateSite(const domain::Site &site);
        [[nodiscard]] std::unique_ptr<domain::Site> GetSite(const std::string site_domain);
        [[nodiscard]] std::unique_ptr<domain::Site> GetSite(const int site_id);
        [[nodiscard]] std::vector<domain::Site> GetAllSites();
        [[nodiscard]] bool UpdateSite(const domain::User &user);
        [[nodiscard]] bool DeleteSite(const domain::User &user);

        // no copy assignment, no empty assignment
        Db() = delete;
        Db (const Db&) = delete;
        Db& operator= (const Db&) = delete;
    private:
        bool initialize();
        void migrateDb(int32_t current_migration_number);
        static std::vector<std::function<void(DatabasePtr &)>> _migrations;
        void clearDb();
        void seedDb(int32_t current_seed_hash);
        nlohmann::json seedData();
        void insertSeedData(const nlohmann::json &seed);
        const bool _seed_db;
        const bool _clear_db;
        const std::filesystem::path _db_path;
        const std::string _db_name;
        DatabasePtr _db = nullptr;
    };
}

#endif //INFRASTRUCTURE_DB_HPP
