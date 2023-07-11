//
// Created by broosegoose on 7/10/23.
//

#include "db.hpp"

using namespace nlohmann::literals;

namespace infrastructure {

    nlohmann::json Db::seedData() {
        /* this isn't safe, but we'll have a test to make sure its valid so eh */
        return R"({
            "sites": [
                {
                    "id": 0,
                    "name": "Admin",
                    "subdomain": "*"
                },
                {
                    "id": 1,
                    "name": "Thompson Lite Brite",
                    "subdomain": "thompson"
                }
            ],
            "users": [
                {
                    "id": 0,
                    "email": "bruce@polis.tv",
                    "password": "broosegoose",
                    "salt": "",
                    "needs_password_change": true,
                    "is_admin": true,
                    "site_id": 0
                },
                {
                    "id": 1,
                    "email": "nate.hardesty@thompsonhotels.com",
                    "password": "thompson",
                    "salt": "",
                    "needs_password_change": true,
                    "is_admin": true,
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
        const auto sites = GetAllSites();
        for (const auto& user : data["users"]) {
            auto domain_user = domain::User::from_json(user);
            domain_user.site_id = sites.at(domain_user.site_id).id;
            const auto ret = CreateUser(domain_user);
            if (!ret) {
                throw std::runtime_error("Failed to insert user");
            }
        }
    }

}