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
#include "domain/display.hpp"


namespace infrastructure {


    typedef std::unique_ptr<SQLite::Database> DatabasePtr;

    struct DbConfig {
        std::filesystem::path db_path;
        std::string db_name;
        bool db_clear;
        bool db_seed;

        // this is SUPER HACKY, but will work for now. Need to just switch to oauth2 or something
        static DbConfig from_json(const nlohmann::json& j) {
            DbConfig conf{};
            conf.db_path = j.value("db_path", ROOT_DIR),
            conf.db_name = j.at("db_name").get<std::string>();
            conf.db_clear = j.at("db_clear").get<bool>();
            conf.db_seed = j.at("db_seed").get<bool>();
            return conf;
        }
    };

    class Db;
    typedef std::shared_ptr<Db> DbPtr;

    struct DbManager {};
    typedef std::shared_ptr<DbManager> DbManagerPtr;

    class Db: public std::enable_shared_from_this<Db> {
    public:
        [[nodiscard]] static DbPtr Create(const DbConfig &conf, DbManagerPtr manager);
        explicit Db(const DbConfig &conf, DbManagerPtr manager);

        // public for test; and I'm not fancy enough to protect friend it with some test class...
        [[nodiscard]] int32_t hashSeedData(const nlohmann::json &seed);

        /* User CRUD */
        [[nodiscard]] bool CreateUser(const domain::User &user);
        [[nodiscard]] bool CreateOrUpdateUser(const domain::User &user);
        // gets return nullptr on failure

        [[nodiscard]] domain::UserPtr GetUser(const std::string &user_email);
        [[nodiscard]] domain::UserPtr GetUser(const int user_id);
        [[nodiscard]] domain::UserPtr GetUserWithCredentials(const int user_id);

        [[nodiscard]] bool UpdateUser(const domain::User &user);
        [[nodiscard]] bool DeleteUser(const domain::User &user);
        [[nodiscard]] bool DeleteUser(const int user_id);

        /* Site CRUD */
        [[nodiscard]] bool CreateSite(const domain::Site &site);
        [[nodiscard]] bool CreateOrUpdateSite(const domain::Site &site);
        [[nodiscard]] domain::SitePtr GetSite(const std::string site_domain);
        [[nodiscard]] domain::SitePtr GetSite(const int site_id);
        [[nodiscard]] std::vector<domain::Site> GetAllSites();
        [[nodiscard]] bool UpdateSite(const domain::User &user);
        [[nodiscard]] bool DeleteSite(const domain::User &user);

        /* Display CRUD */
        [[nodiscard]] bool CreateDisplay(const domain::Display &display, const int site_id);
        [[nodiscard]] bool CreateOrUpdateDisplay(const domain::Display &display, const int site_id);
        [[nodiscard]] domain::DisplayPtr GetDisplay(const int site_id);
        [[nodiscard]] bool DeleteDisplay(const int site_id);

        // no copy assignment, no empty assignment
        Db() = delete;
        Db (const Db&) = delete;
        Db& operator= (const Db&) = delete;
    private:
        bool initialize(const DbConfig &conf);
        void migrateDb(int32_t current_migration_number);
        static std::vector<std::function<void(DatabasePtr &)>> _migrations;
        void clearDb();
        void seedDb(int32_t current_seed_hash);
        nlohmann::json seedData();
        void insertSeedData(const nlohmann::json &seed);
        const std::filesystem::path _db_path;
        const std::string _db_name;
        DbManagerPtr _manager;
        DatabasePtr _db = nullptr;
    };
}

#endif //INFRASTRUCTURE_DB_HPP
