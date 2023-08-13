//
// Created by broosegoose on 7/9/23.
//

#include <iostream>

#include "db.hpp"

#include "utils/files.hpp"

namespace infrastructure {

    DbPtr Db::Create(const DbConfig &conf, DbManagerPtr manager) {
        auto db = std::make_shared<Db>(conf, std::move(manager));
        if (!db->initialize(conf)) {
            return nullptr;
        }
        return std::move(db);
    }

    Db::Db(const infrastructure::DbConfig &conf, DbManagerPtr manager):
            _db_path(conf.db_path), _db_name(conf.db_name), _manager(std::move(manager))
    {}

    bool Db::initialize(const DbConfig &conf) {
        // check if the path is valid
        const auto valid_dir = utils::IsValidDirectory(_db_path);
        if (!valid_dir) {
            return false;
        }
        const auto db_file = _db_path / _db_name;
        const auto ternary_file_exists = utils::FileExists(db_file);
        if (ternary_file_exists == utils::Ternary::ERR) {
            return false;
        }
        try {
            int32_t migration_number = 0;
            int32_t seed_hash = 0;
            const bool db_exists = ternary_file_exists == utils::Ternary::TRUE;
            if (db_exists) {
                // this will throw if the file was invalid
                const auto header = SQLite::Database::getHeaderInfo(db_file);
                migration_number = header.userVersion;
                seed_hash = header.applicationId;
            }
            _db = std::make_unique<SQLite::Database>(db_file, SQLite::OPEN_READWRITE|SQLite::OPEN_CREATE);
            (*_db).exec("PRAGMA foreign_keys = ON;");
            migrateDb(migration_number);
            if (conf.db_clear && db_exists) {
                clearDb();
                seed_hash = 0;
            }
            if (conf.db_seed) {
                seedDb(seed_hash);
            }
            return true;
        } catch (std::exception &e) {
            std::cout << e.what() << std::endl;
            return false;
        }
    }

    void Db::migrateDb(const int32_t current_migration_number) {
        const auto migration_count = Db::_migrations.size();
        if (current_migration_number > migration_count) {
            throw std::runtime_error("Migrations lost a hommie");
        } else if (current_migration_number == migration_count) {
            return;
        }
        auto begin = Db::_migrations.begin() + current_migration_number;
        auto end = Db::_migrations.end();
        for (auto it = begin; it != end; ++it) {
            (*it)(_db);
        }
        SQLite::Statement update(*_db, "PRAGMA user_version = " + std::to_string(migration_count) + ";");
        update.exec();
    }

    void Db::seedDb(const int32_t current_seed_hash) {
        const auto seed = seedData();
        const auto seed_data_hash = hashSeedData(seed);
        if (seed_data_hash != current_seed_hash) {
            clearDb();
            insertSeedData(seed);
            SQLite::Statement update(*_db, "PRAGMA application_id = " +  std::to_string(seed_data_hash) + ";");
            update.exec();
        }
    }
}