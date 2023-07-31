//
// Created by broosegoose on 7/9/23.
//

#include <doctest.h>
#include <thread>
#include <optional>
#include <iostream>

#include "utils/clock.hpp"

#include "domain/user.hpp"

#include "infrastructure/auth/auth.hpp"

[[nodiscard]] infrastructure::AuthConfig GetAuthTestConfig(
    std::optional<utils::Duration> expiry,
    std::optional<utils::Duration> refresh
) {
    return {
        .auth_pepper = "TEST_PEPPER",
        .auth_jwt_secret = "TEST_SECRET",
        .auth_jwt_expiry = expiry.has_value() ? expiry.value() : 30min,
        .auth_jwt_refresh = refresh.has_value() ? refresh.value() : 5min
    };
}

TEST_CASE("Infrastructure_Auth-Creation") {
    const auto conf = GetAuthTestConfig({}, {});
    auto auth = infrastructure::Auth::Create(conf);
    REQUIRE((bool)(auth != nullptr));
}

TEST_CASE("Infrastructure_Auth-HashPassword") {
    const auto conf = GetAuthTestConfig({}, {});
    auto auth = infrastructure::Auth::Create(conf);

    const std::string password = "test-password";
    auto create_user = domain::User {
        .id = 1,
        .email = "bruce@polis.tv",
        .password = password
    };
    const auto success = auth->HashPassword(create_user);

    // make sure it worked
    REQUIRE(success);
    // make sure it replaced the current password
    REQUIRE_NE(password, create_user.password);
    // make sure it set the salt
    REQUIRE_NE(create_user.salt, "");

}

TEST_CASE("Infrastructure_Auth-PasswordRetrieval") {
    const auto conf = GetAuthTestConfig({}, {});
    auto auth = infrastructure::Auth::Create(conf);

    auto db_user = domain::User {
            .id = 1,
            .email = "bruce@polis.tv",
            .name = "broose",
            .password = "test-password"
    };
    auto login_user = db_user;
    const auto success = auth->HashPassword(db_user);
    // make sure we aren't insane
    REQUIRE_NE(login_user.password, db_user.password);

    auto ret = auth->VerifyLogin(login_user, db_user);
    // make sure we succeeded
    REQUIRE(ret.first);
    std::cout << ret.second << std::endl;
    // should check that we returned a valid jwt; but for now we don't do that

    login_user.password = "some-different-password";
    ret = auth->VerifyLogin(login_user, db_user);
    // make sure it failed with the wrong password
    REQUIRE_FALSE(ret.first);
}

TEST_CASE("Infrastructure_Auth-JwtStore") {
    const auto conf = GetAuthTestConfig({}, {});
    auto auth = infrastructure::Auth::Create(conf);

    auto db_user = domain::User {
            .id = 70,
            .email = "bruce@polis.tv",
            .name = "broose",
            .password = "test-password",
            .is_admin = true,
            .site_id = 33
    };
    auto login_user = db_user;
    auto success = auth->HashPassword(db_user);
    auto verify_ret = auth->VerifyLogin(login_user, db_user);
    auto token = verify_ret.second;

    // make sure we get the user back out from the jwt
    auto [validate_success, new_token, validate_user_id] = auth->ValidateToken(token);
    // make sure it worked
    REQUIRE(validate_success);
    REQUIRE(new_token.empty());
    REQUIRE_NE(validate_user_id, -1);
    // make sure the user matches
    REQUIRE_EQ(validate_user_id, login_user.id);

    /* mess up the token; make sure that fails */
    auto messed_up_token = token;
    std::swap(messed_up_token[1], messed_up_token[4]);
    auto [should_fail, bad_token, bad_user_id] = auth->ValidateToken(messed_up_token);
    REQUIRE_FALSE(should_fail);
    REQUIRE(bad_token.empty());
    REQUIRE_EQ(bad_user_id, -1);
}

TEST_CASE("Infrastructure_Auth-JwtStoreRefresh") {
    const auto conf = GetAuthTestConfig({30min}, {30min});
    auto auth = infrastructure::Auth::Create(conf);

    auto db_user = domain::User {
            .id = 70,
            .email = "bruce@polis.tv",
            .name = "broose",
            .password = "test-password",
            .is_admin = true,
            .site_id = 33
    };
    auto login_user = db_user;

    db_user.password = login_user.password;
    auto success = auth->HashPassword(db_user);
    auto verify_ret = auth->VerifyLogin(login_user, db_user);
    auto old_token = verify_ret.second;

    // wait for the token to reach refresh
    std::this_thread::sleep_for(1ms);

    // make sure we get the user back out from the jwt
    auto [validate_success, refresh_token, validate_user_id] = auth->ValidateToken(verify_ret.second);
    REQUIRE(validate_success);
    REQUIRE_NE(validate_user_id, -1);
    // token should have updated
    REQUIRE_NE(refresh_token, old_token);
}

TEST_CASE("Infrastructure_Auth-JwtExpiry") {
    const auto conf = GetAuthTestConfig({0s}, {0s});
    auto auth = infrastructure::Auth::Create(conf);

    auto db_user = domain::User {
            .id = 70,
            .email = "bruce@polis.tv",
            .name = "broose",
            .password = "test-password",
            .is_admin = true,
            .site_id = 33
    };
    auto login_user = db_user;
    auto success = auth->HashPassword(db_user);
    auto verify_ret = auth->VerifyLogin(login_user, db_user);

    // wait for the token to reach expiry
    std::this_thread::sleep_for(1ms);

    // make sure it failed
    auto [should_fail, should_be_empty, should_be_bad_user_id] = auth->ValidateToken(verify_ret.second);
    REQUIRE_FALSE(should_fail);
    REQUIRE(should_be_empty.empty());
    REQUIRE_EQ(should_be_bad_user_id, -1);
}

TEST_CASE("Infrastructure_Auth-GetPepperedPassword") {
    auto conf = GetAuthTestConfig({}, {});
    // YOU BETTER NOT COMMIT A FKN PEPPER HERE
    conf.auth_pepper = "";
    auto auth = infrastructure::Auth::Create(conf);
    // YOU BETTER NOT COMMIT A FKN REAL PASSWORD HERE
    auto user = domain::User {
        .password = "",
    };
    const auto success = auth->HashPassword(user);
    REQUIRE(success);
    std::cout << "Password: " << user.password << std::endl;
    std::cout << "Salt: " << user.salt << std::endl;

}