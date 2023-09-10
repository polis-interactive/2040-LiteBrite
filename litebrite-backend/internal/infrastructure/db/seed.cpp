//
// Created by broosegoose on 7/10/23.
//

#include "db.hpp"

using namespace nlohmann::literals;

namespace infrastructure {

    nlohmann::json Db::seedData() {
        return R"({
            "sites": [
                {
                    "id": 0,
                    "name": "Admin",
                    "slug": "admin"
                },
                {
                    "id": 1,
                    "name": "Thompson Lite Brite",
                    "slug": "thompson-lite-brite"
                }
            ],
            "users": [
                {
                    "id": 0,
                    "email": "bruce@polis.tv",
                    "name": "Broose Goose",
                    "is_admin": true,
                    "site_id": 0
                },
                {
                    "id": 1,
                    "email": "nate.hardesty@thompsonhotels.com",
                    "name": "Nate Hardesty",
                    "is_admin": false,
                    "site_id": 1
                }
            ]
        })"_json;;
    }

    int32_t Db::hashSeedData(const nlohmann::json &seed) {
        std::hash<nlohmann::json> hasher;
        return (int32_t) hasher(seed);
    }

    void Db::insertSeedData(const nlohmann::json &data) {
        for (const auto& site : data["sites"]) {
            const auto domain_site = domain::Site::from_json(site);
            const auto ret = CreateSite(domain_site);
            if (!ret) {
                throw std::runtime_error("Failed to insert site");
            }
        }
        for (const auto& user : data["users"]) {
            auto domain_user = domain::User::from_json(user);
            const auto ret = CreateUser(domain_user);
            if (!ret) {
                throw std::runtime_error("Failed to insert user");
            }
        }
    }

}