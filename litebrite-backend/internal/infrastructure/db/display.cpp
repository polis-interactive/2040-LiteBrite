//
// Created by broosegoose on 9/19/23.
//

#include "db.hpp"

#include <iostream>

namespace infrastructure {

    bool Db::CreateOrUpdateDisplay(const domain::Display &display, const int site_id) {
        SQLite::Statement query(
            *_db, R"(
                INSERT INTO displays (site_id, display) VALUES (?, ?)
                ON CONFLICT(site_id) DO UPDATE SET display = ?
            )"
        );
        const auto display_string = to_string(display.to_json());
        query.bind(1, site_id);
        query.bind(2, display_string);
        query.bind(3, display_string);
        int result = query.exec();
        return result == 1;
    }

    domain::DisplayPtr Db::GetDisplay(const int site_id) {
        SQLite::Statement query(*_db, "SELECT display FROM displays WHERE site_id = ?");
        query.bind(1, site_id);
        if (query.executeStep()) {
            try {
                std::string display_string = query.getColumn("display");
                nlohmann::json j = nlohmann::json::parse(display_string);
                auto display = domain::Display::from_json(j);
                return std::make_shared<domain::Display>(display);
            } catch (nlohmann::json::exception& e) {
                std::cerr << "Db::GetDisplay - json error: " << e.what() << std::endl;
            } catch(std::runtime_error &e) {
                std::cerr << "Db::GetDisplay - runtime error: " << e.what() << std::endl;
            } catch (...) {
               std::cerr << "Db::GetDisplay - Unknown runtime error" << std::endl;
            }
        }
        return nullptr;
    }

    bool Db::DeleteDisplay(const int site_id) {
        SQLite::Statement query(*_db, "DELETE FROM displays WHERE site_id = ?");
        query.bind(1, site_id);
        query.executeStep();
        return true;
    }
}