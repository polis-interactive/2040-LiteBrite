//
// Created by broosegoose on 7/9/23.
//

#include <doctest.h>
#include <thread>
#include <optional>
#include <algorithm>
#include <iostream>

#include "utils/clock.hpp"

#include "domain/user.hpp"

#include "infrastructure/auth/auth.hpp"

[[nodiscard]] infrastructure::AuthConfig GetTextConfig(
    std::optional<utils::duration> expiry,
    std::optional<utils::duration> refresh
) {
    return {
        .pepper = "TEST_PEPPER",
        .jwt_secret = "TEST_SECRET",
        .jwt_expiry = expiry.has_value() ? expiry.value() : 30min,
        .jwt_refresh = refresh.has_value() ? refresh.value() : 5min

    };
}

TEST_CASE("Infrastructure_Auth-Creation") {
    const auto conf = GetTextConfig({}, {});
    std::shared_ptr<infrastructure::Auth> auth = infrastructure::Auth::Create(conf);
    REQUIRE((bool)(auth != nullptr));
}

TEST_CASE("Infrastructure_Auth-HashPassword") {
    const auto conf = GetTextConfig({}, {});
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
    const auto conf = GetTextConfig({}, {});
    auto auth = infrastructure::Auth::Create(conf);

    auto db_user = domain::User {
            .id = 1,
            .email = "bruce@polis.tv",
            .password = "test-password"
    };
    auto login_user = db_user;
    const auto success = auth->HashPassword(db_user);
    // make sure we aren't insane
    REQUIRE_NE(login_user.password, db_user.password);

    auto ret = auth->VerifyLogin(login_user, db_user);
    // make sure we succeeded
    REQUIRE(ret.first);
    // should check that we returned a valid jwt; but for now we don't do that

    login_user.password = "some-different-password";
    ret = auth->VerifyLogin(login_user, db_user);
    // make sure it failed with the wrong password
    REQUIRE_FALSE(ret.first);
}

TEST_CASE("Infrastructure_Auth-JwtStore") {
    const auto conf = GetTextConfig({}, {});
    auto auth = infrastructure::Auth::Create(conf);

    auto db_user = domain::User {
            .id = 70,
            .email = "bruce@polis.tv",
            .password = "test-password",
            .is_admin = true,
            .site_id = 33
    };
    auto login_user = db_user;
    auto success = auth->HashPassword(db_user);
    auto verify_ret = auth->VerifyLogin(login_user, db_user);
    auto token = verify_ret.second;

    // make sure we get the user back out from the jwt
    auto token_ret = auth->ValidateToken(token);
    // make sure it worked
    REQUIRE(token_ret.first);
    REQUIRE((bool)(token_ret.second != nullptr));
    auto out_user = std::move(token_ret.second);
    // make sure the user matches
    REQUIRE_EQ(out_user->id, login_user.id);
    REQUIRE_EQ(out_user->is_admin, login_user.is_admin);
    REQUIRE_EQ(out_user->site_id, login_user.site_id);

    /* mess up the token; make sure that fails */
    auto messed_up_token = token;
    std::swap(messed_up_token[1], messed_up_token[4]);
    token_ret = auth->ValidateToken(messed_up_token);
    REQUIRE_FALSE(token_ret.first);
    REQUIRE((bool)(token_ret.second == nullptr));
}

TEST_CASE("Infrastructure_Auth-JwtStoreRefresh") {

    const auto conf = GetTextConfig({ 30min }, { 30min });
    auto auth = infrastructure::Auth::Create(conf);

    auto db_user = domain::User {
            .id = 70,
            .email = "bruce@polis.tv",
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
    auto token_ret = auth->ValidateToken(verify_ret.second);
    REQUIRE(token_ret.first);
    REQUIRE((bool)(token_ret.second != nullptr));
    // token should have updated
    REQUIRE_NE(verify_ret.second, old_token);
}

TEST_CASE("Infrastructure_Auth-JwtExpiry") {

    const auto conf = GetTextConfig({ 0s }, { 0s });
    auto auth = infrastructure::Auth::Create(conf);

    auto db_user = domain::User {
            .id = 70,
            .email = "bruce@polis.tv",
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
    auto token_ret = auth->ValidateToken(verify_ret.second);
    REQUIRE_FALSE(token_ret.first);
    REQUIRE((bool)(token_ret.second == nullptr));
}