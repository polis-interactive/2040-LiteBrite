//
// Created by broosegoose on 7/11/23.
//

#include "db.hpp"

namespace infrastructure {

    bool Db::CreateSite(const domain::Site &site) {
        SQLite::Statement query(
            *_db, "INSERT INTO sites (id, name, slug) VALUES (?, ?, ?)"
        );
        query.bind(1, site.id);
        query.bind(2, site.name);
        query.bind(3, site.slug);
        int result = query.exec();  // Executes the insert operation
        return result == 1;
    }

    std::vector<domain::Site> Db::GetAllSites() {
        std::vector<domain::Site> sites;
        SQLite::Statement query(*_db, "SELECT * FROM sites;");
        while (query.executeStep()) {
            domain::Site site;
            site.id = query.getColumn("id");
            site.name = query.getColumn("name").getText();
            site.slug = query.getColumn("slug").getText();
            sites.push_back(site);
        }
        return sites;
    }

    domain::SitePtr Db::GetSite(const int site_id) {
        SQLite::Statement query(*_db, "SELECT * FROM sites WHERE id = ?");
        query.bind(1, site_id);

        if (query.executeStep()) {
            auto site = std::make_unique<domain::Site>();
            site->id = query.getColumn("id");
            site->name = query.getColumn("name").getString();
            site->slug = query.getColumn("slug").getString();
            return site;
        }

        return nullptr;
    }

}