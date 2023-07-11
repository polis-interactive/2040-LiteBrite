//
// Created by broosegoose on 7/9/23.
//

#include "db.hpp"

#include "utils/files.hpp"

/*
 * I'll put migrations up here; or maybe I'll have a migrations header / cpp file
 */

namespace infrastructure {

    std::shared_ptr<Db> Db::Create(const DbConfig &conf) {
        auto db = std::make_shared<Db>(conf);
        if (!db->initialize()) {
            return nullptr;
        }
        return std::move(db);
    }

    Db::Db(const infrastructure::DbConfig &conf):
        _db_path(conf.db_path), _db_name(conf.db_name), _seed_db(conf.seed_db)
    {}

    bool Db::initialize() {
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
            _db = std::make_unique<SQLite::Database>(db_file, SQLite::OPEN_READWRITE|SQLite::OPEN_CREATE);
            (*_db).exec("PRAGMA foreign_keys = ON;");
            migrateDb();
            if (_clear_db) {
                clearDb();
            }
            if (_seed_db) {
                seedDb();
            }
            return true;
        } catch (std::exception &e) {
            return false;
        }
    }

    void Db::migrateDb() {
        SQLite::Statement query(*_db, "PRAGMA user_version;");
        int32_t version = 0;
        if (query.executeStep()) {
            version = query.getColumn(0);
        }
        if (version > Db::_migrations.size()) {
            throw std::runtime_error("Migrations lost a hommie");
        }
        auto begin = Db::_migrations.begin() + version;
        auto end = Db::_migrations.end();
        for (auto it = begin; it != end; ++it) {
            (*it)(_db);
        }
        SQLite::Statement update(*_db, "PRAGMA user_version = ?;");
        update.bind(1, (int32_t) Db::_migrations.size());
        update.exec();
    }

    void Db::seedDb() {
        const auto seed = seedData();
        const auto seedDataHash = hashSeedData(seed);

        SQLite::Statement query(*_db, "PRAGMA application_id;");
        int32_t storedHash = 0;
        if (query.executeStep()) {
            storedHash = query.getColumn(0);
        }

        if (seedDataHash != storedHash) {
            clearDb();
            insertSeedData(seed);
            SQLite::Statement update(*_db, "PRAGMA application_id = ?;");
            update.bind(1, seedDataHash);
            update.exec();
        }

    }
}