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
        .db_clear = false,
        .db_seed = true,
    };
}

struct TestDbManager:
    public infrastructure::DbManager,
    public std::enable_shared_from_this<TestDbManager>
{};

TEST_CASE("Infrastructure_Db-Creation") {
    const auto conf = GetDbTestConf();
    auto manager = std::make_shared<TestDbManager>();
    std::filesystem::remove(conf.db_path / conf.db_name);
    {
        auto db = infrastructure::Db::Create(conf, manager->shared_from_this());
        REQUIRE((bool)(db != nullptr));
    }
    {
        // right now, just visually inspect things printing saying nothing happened
        auto db = infrastructure::Db::Create(conf, manager->shared_from_this());
        std::this_thread::sleep_for(1s);
        REQUIRE((bool)(db != nullptr));
    }
}