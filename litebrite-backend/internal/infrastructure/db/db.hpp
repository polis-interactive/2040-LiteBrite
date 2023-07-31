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
        bool db_clear;
        bool db_seed;
        std::string broose_password;
        std::string thompson_password;

        // this is SUPER HACKY, but will work for now. Need to just switch to oauth2 or something
        static DbConfig from_source(
            const nlohmann::json& j, const std::string& broose_password, const std::string &thompson_password
        ) {
            DbConfig conf{};
            if (broose_password.empty()) {
                throw std::domain_error("DB_BROOSE_PASSWORD_RAW ENV UNSET");
            } else if (thompson_password.empty()) {
                throw std::domain_error("DB_THOMPSON_PASSWORD_RAW ENV UNSET");
            }
            conf.broose_password = broose_password;
            conf.thompson_password = thompson_password;
            conf.db_path = j.value("db_path", ROOT_DIR),
            conf.db_name = j.at("db_name").get<std::string>();
            conf.db_clear = j.at("db_clear").get<bool>();
            conf.db_seed = j.at("db_seed").get<bool>();
            return conf;
        }
    };

    class Db;
    typedef std::shared_ptr<Db> DbPtr;

    struct DbManager {
        virtual bool HashPassword(domain::User &user) = 0;
    };
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
        [[nodiscard]] std::unique_ptr<domain::User> GetUser(const std::string &user_email);
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
        bool initialize(const DbConfig &conf);
        void migrateDb(int32_t current_migration_number);
        static std::vector<std::function<void(DatabasePtr &)>> _migrations;
        void clearDb();
        void seedDb(int32_t current_seed_hash, const DbConfig &conf);
        nlohmann::json seedData();
        void insertSeedData(const nlohmann::json &seed, const DbConfig &conf);
        const std::filesystem::path _db_path;
        const std::string _db_name;
        DbManagerPtr _manager;
        DatabasePtr _db = nullptr;
    };
}

#endif //INFRASTRUCTURE_DB_HPP
