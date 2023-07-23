//
// Created by broosegoose on 7/9/23.
//

#ifndef INFRASTRUCTURE_AUTH_HPP
#define INFRASTRUCTURE_AUTH_HPP

#include <string>
#include <memory>
#include <set>
#include <mutex>
#include <shared_mutex>
#include <atomic>

#include "utils/clock.hpp"

#include "domain/user.hpp"


namespace infrastructure {

    struct AuthConfig {
        std::string auth_pepper;
        std::string auth_jwt_secret;
        utils::Duration auth_jwt_expiry;
        utils::Duration auth_jwt_refresh;
        static AuthConfig from_source(
            const nlohmann::json& j, const std::string& pepper, const std::string &jwt_secret
        ) {
            AuthConfig conf{};
            conf.auth_pepper = pepper;
            conf.auth_jwt_secret = jwt_secret;
            conf.auth_jwt_expiry = std::chrono::seconds(j.at("auth_jwt_expiry").get<unsigned int>());
            conf.auth_jwt_refresh = std::chrono::seconds(j.at("auth_jwt_refresh").get<unsigned int>());
            return conf;
        }
    };

    class Auth;
    typedef std::shared_ptr<Auth> AuthPtr;

    class Auth {
    public:
        [[nodiscard]] static AuthPtr Create(const AuthConfig &conf);
        explicit Auth(const AuthConfig &conf);

        /* hashes a users password, creates a salt to store in db */
        [[nodiscard]] bool HashPassword(domain::User &user);

        /* generates a jwt on success; error message on failure */
        [[nodiscard]] std::pair<bool, std::string> VerifyLogin(
            const domain::User &login, const domain::User &store
        );

        /* parse jwt;
         *      check it if's known
         *      if it's valid
         *      if it's about to expire, update the passed jwt
         *      if it's invalid, SAY SO
         *      return a user if we can
         */
        [[nodiscard]] std::pair<bool, std::unique_ptr<domain::User>> ValidateToken(std::string &jwt);

        // no copy assignment, no empty assignment
        Auth() = delete;
        Auth (const Auth&) = delete;
        Auth& operator= (const Auth&) = delete;
    private:
        bool initialize();
        [[nodiscard]] std::string CreateJwt(const domain::User &user);

        mutable std::shared_mutex _jwt_mutex;
        std::set<std::string> _jwt_store;
        std::atomic<unsigned long> _jwt_counter = { 0 };

        const std::string _pepper;
        const std::string _jwt_secret;
        const utils::Duration _jwt_expiry;
        const utils::Duration _jwt_refresh;
    };
}

#endif //INFRASTRUCTURE_AUTH_HPP
