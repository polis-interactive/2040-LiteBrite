//
// Created by broosegoose on 7/9/23.
//

#include "auth.hpp"

#include <sodium.h>
#include <iostream>

#include "jwt-cpp/traits/nlohmann-json/traits.h"

using traits = jwt::traits::nlohmann_json;
using claim = jwt::basic_claim<traits>;


namespace infrastructure {

    AuthPtr Auth::Create(const AuthConfig &conf) {
        auto server = std::make_shared<Auth>(conf);
        if (!server->initialize()) {
            return nullptr;
        }
        return std::move(server);
    }

    Auth::Auth(const infrastructure::AuthConfig &conf):
        _pepper(conf.auth_pepper), _jwt_secret(conf.auth_jwt_secret), _jwt_refresh(conf.auth_jwt_refresh),
        _jwt_expiry(conf.auth_jwt_expiry)
    {}

    bool Auth::initialize() {
        if (sodium_init() < 0) {
            // failed to initialize libsodium
            return false;
        }
        return true;
    }

    bool Auth::HashPassword(domain::User &user) {
        /* new salt */
        std::string salt (crypto_pwhash_SALTBYTES, 'x');
        randombytes_buf(salt.data(), crypto_pwhash_SALTBYTES);

        /* add salt and pepper */
        const std::string final_password = user.password + salt + _pepper;

        /* generate output hash */
        char hashed_password[crypto_pwhash_STRBYTES];
        const int err = crypto_pwhash_str(
            hashed_password, final_password.c_str(), final_password.length(),
            crypto_pwhash_OPSLIMIT_INTERACTIVE,
            crypto_pwhash_MEMLIMIT_INTERACTIVE
        );
        if (err != 0) {
            return false;
        }
        user.password = hashed_password;
        user.salt = salt;
        return true;
    }

    std::pair<bool, std::string> Auth::VerifyLogin(const domain::User &login, const domain::User &store) {
        if (!store.is_admin && login.site_id != store.site_id) {
            return { false, "No Access" };
        }
        const auto login_password = login.password + store.salt + _pepper;
        const auto verified = crypto_pwhash_str_verify(
            store.password.c_str(), login_password.c_str(), login_password.length()
        );
        if (verified != 0) {
            return { false, "Invalid Password" };
        }
        auto jwt_token = CreateJwt(store);
        return { true, jwt_token };
    }

    [[nodiscard]] std::string Auth::CreateJwt(const domain::User &user) {
        const auto now = utils::Clock::now();
        auto token = jwt::create<traits>()
                .set_type("JWT")
                .set_issuer("lighting.polis.tv")
                .set_audience("*.lighting.polis.tv")
                .set_id(std::to_string(_jwt_counter.fetch_add(1)))
                .set_issued_at(now)
                .set_not_before(now)
                .set_expires_at(now + _jwt_expiry)
                .set_payload_claim("id", user.id)
                .sign(jwt::algorithm::hs256{_jwt_secret});

        std::unique_lock lk(_jwt_mutex);
        _jwt_store.insert(token);
        return token;
    }

    std::tuple<bool, std::string, int> Auth::ValidateToken(const std::string &jwt) {
        /* check if the jwt is known; if not, reject it */
        bool found_jwt = false;
        {
            std::shared_lock lk(_jwt_mutex);
            if (_jwt_store.find(jwt) != _jwt_store.end()) {
                found_jwt = true;
            }
        }
        if (!found_jwt) {
            return { false, "", -1 };
        }

        try {
            auto decoded_token = jwt::decode<traits>(jwt);

            // will throw if invalid
            jwt::verify<traits>()
                .allow_algorithm(jwt::algorithm::hs256{_jwt_secret})
                .with_issuer("lighting.polis.tv")
                .with_audience("*.lighting.polis.tv")
                .verify(decoded_token);

            // get the user back
            auto user = std::make_unique<domain::User>();
            user->id = decoded_token.get_payload_claim("id").as_integer();

            // Check if token is about to expire / has expired
            auto now = utils::Clock::now();
            auto expiry = decoded_token.get_expires_at();
            if (expiry - now <= _jwt_refresh) {
                RemoveJwt(jwt);
                // refresh the jwt; delete the old one
                auto new_jwt = CreateJwt(*user);
                return {true, new_jwt, user->id};
            } else {
                return { true, "", user->id };
            }
        }
        catch (const jwt::error::token_verification_exception& e) {
            RemoveJwt(jwt);
            return { false, "", -1 };
        }
    }

    void Auth::RemoveJwt(const std::string &jwt) {
        std::unique_lock lk(_jwt_mutex);
        _jwt_store.erase(jwt);
    }

}