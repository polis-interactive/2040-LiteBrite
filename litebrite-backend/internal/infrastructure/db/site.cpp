//
// Created by broosegoose on 7/11/23.
//

#include "db.hpp"

namespace infrastructure {

    bool Db::CreateSite(const domain::Site &site) {
        SQLite::Statement query(
            *_db, "INSERT INTO sites (name, subdomain) VALUES (?, ?)"
        );
        query.bind(1, site.name);
        query.bind(2, site.subdomain);
        int result = query.exec();  // Executes the insert operation
        return result == 1;
    }

    std::vector<domain::Site> Db::GetAllSites() {
        std::vector<domain::Site> sites;
        SQLite::Statement query(*_db, "SELECT * FROM sites;");
        while (query.executeStep()) {
            domain::Site site;
            site.id = query.getColumn(0);
            site.subdomain = query.getColumn(1).getText();
            site.name = query.getColumn(2).getText();
            sites.push_back(site);
        }
        return sites;
    }

}