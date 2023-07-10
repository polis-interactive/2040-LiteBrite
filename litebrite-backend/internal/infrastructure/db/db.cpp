//
// Created by broosegoose on 7/9/23.
//

#include "db.hpp"

#include "utils/files.hpp"

namespace infrastructure {

    std::shared_ptr<Db> Db::Create(const DbConfig &conf) {
        auto db = std::make_shared<Db>(conf);
        if (!db->initialize()) {
            return nullptr;
        }
        return std::move(db);
    }

    Db::Db(const infrastructure::DbConfig &conf):
        _db_path(conf.db_path), _db_name(conf.db_name)
    {}

    bool Db::initialize() {
        // check if the path is valid
        const auto valid_dir = utils::IsValidDirectory(_db_path);
        if (!valid_dir) {
            return false;
        }
        const auto db_file = valid_dir / _db_name;
    }
}