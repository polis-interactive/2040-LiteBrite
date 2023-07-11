//
// Created by broosegoose on 7/10/23.
//

#include "db.hpp"

namespace infrastructure {

    nlohmann::json Db::seedData() {
        nlohmann::json seedData;
        return seedData;
    }

    int32_t Db::hashSeedData(const nlohmann::json &seed) {
        std::hash<nlohmann::json> hasher;
        return (int32_t) hasher(seed);
    }

    void Db::insertSeedData(const nlohmann::json &seed) {

    }

}