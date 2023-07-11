//
// Created by broosegoose on 7/10/23.
//

#include <doctest.h>
#include <thread>
#include <iostream>

#include "utils/clock.hpp"

#include "infrastructure/db/db.hpp"

[[nodiscard]] infrastructure::DbConfig GetDbTestConf() {
    return {
        .db_path = TEST_DIR,
        .db_name = "test.db",
        .clear_db = false,
        .seed_db = true,
    };
}

TEST_CASE("Infrastructure_Auth-Creation") {
    const auto conf = GetDbTestConf();
    std::filesystem::remove(conf.db_path / conf.db_name);
    {
        auto db = infrastructure::Db::Create(conf);
        REQUIRE((bool)(db != nullptr));
    }
    {
        // right now, just visually inspect things printing saying nothing happened
        auto db = infrastructure::Db::Create(conf);
        std::this_thread::sleep_for(1s);
        REQUIRE((bool)(db != nullptr));
    }
}