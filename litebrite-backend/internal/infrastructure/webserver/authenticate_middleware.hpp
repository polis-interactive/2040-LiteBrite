//
// Created by broosegoose on 8/3/23.
//

#ifndef INFRASTRUCTURE_WEBSERVER_AUTHENTICATE_MIDDLEWARE_HPP
#define INFRASTRUCTURE_WEBSERVER_AUTHENTICATE_MIDDLEWARE_HPP

#include "crow/http_request.h"
#include "crow/http_response.h"
#include "crow/routing.h"

#include "jwt-cpp/traits/nlohmann-json/traits.h"

using traits = jwt::traits::nlohmann_json;
using claim = jwt::basic_claim<traits>;

#define CPPHTTPLIB_OPENSSL_SUPPORT
#include "cpp-httplib/httplib.h"

#include "domain/user.hpp"

#include "infrastructure/db/db.hpp"


namespace crow
{
    struct AuthenticateHandler
    {
        struct context
        {
            domain::UserPtr user = nullptr;
        };

        void before_handle(crow::request& req, crow::response& res, context& ctx)
        {
            if (req.method == "OPTIONS"_method || req.url.substr(0, _public_prefix.size()) == _public_prefix) {
                // No authN needed on preflight / public routes
                return;
            }
            if (!middlewareIsConfigured(res)) {
                return;
            }
            const auto user_email = tryParseAuthToken(req, res);
            if (user_email.empty()) {
                return;
            }
            tryResolveUser(user_email, res, ctx);
        }

        void after_handle(crow::request& /*req*/, crow::response& /*res*/, context& /*ctx*/)
        {}

        AuthenticateHandler& SetAudience(const std::string &audience) {
            _audience = audience;
            return *this;
        }

        AuthenticateHandler& SetIssuer(const std::string &issuer) {
            _issuer = issuer;
            fetchPublicKeys();
            return *this;
        }

        AuthenticateHandler& SetDb(infrastructure::DbPtr db) {
            _db = std::move(db);
            return *this;
        }

    private:

        bool fetchPublicKeys() {
            httplib::Client client(_issuer);
            auto res = client.Get("/.well-known/jwks.json");
            if (res && res->status == 200) {
                return tryParseJwks(res->body);
            } else {
                // Handle error
                if (res) {
                    std::cerr << "AuthenticateMiddleware::fetchPublicKeys: failed with status:" <<
                        res->status << std::endl;
                } else {
                    std::cerr << "AuthenticateMiddleware::fetchPublicKeys: Failed to connect or invalid URL."
                    << std::endl;
                }
            }
            return false;
        }

        bool tryParseJwks(const std::string &raw_jwks) {
            try {
                const auto json = nlohmann::json::parse(raw_jwks);
                const auto &jwks = json["keys"];
                for (const auto& jwk : jwks) {
                    std::string kid = jwk["kid"];
                    std::string x5c = jwt::helper::convert_base64_der_to_pem(jwk["x5c"][0]);
                    _public_keys.insert({ kid, x5c });
                }
                return !_public_keys.empty();
            } catch (nlohmann::json::exception& e) {
                std::cout << "AuthenticateMiddleware::fetchPublicKeys failed to parse json with err:" <<
                    e.what() << std::endl;
                return false;
            }
        }

        bool middlewareIsConfigured(crow::response& res) {
            if (_db == nullptr || _issuer.empty() || _audience.empty()) {
                res.code = crow::status::INTERNAL_SERVER_ERROR;
                res.body = "crow::AuthenticateMiddleware is not properly configured";
                res.end();
                return false;
            }
            if (!_public_keys.empty()) {
                return true;
            }
            if (!fetchPublicKeys()) {
                res.code = crow::status::INTERNAL_SERVER_ERROR;
                res.body = "crow::AuthenticateMiddleware couldn't fetch public keys";
                res.end();
                return false;
            }
            return true;
        }

        std::string tryParseAuthToken(crow::request &req, crow::response &res) {
            const auto auth_header = req.get_header_value("Authorization");
            if (auth_header.substr(0, _token_prefix.size()) != _token_prefix) {
                res.code = crow::status::UNAUTHORIZED;
                res.body = "crow::AuthenticateMiddleware invalid Authorization header";
                res.end();
                return "";
            }
            const auto jwt = auth_header.substr(_token_prefix.size());
            try {
                auto decoded_token = jwt::decode<traits>(jwt);
                const auto header = decoded_token.get_header_json();
                const auto kid_itr = header.find("kid");
                if (kid_itr == header.end()) {
                    throw jwt::error::token_verification_exception();
                }
                const auto pkey_itr = _public_keys.find(kid_itr->second);
                if (pkey_itr == _public_keys.end()) {
                    throw jwt::error::token_verification_exception();
                }
                jwt::verify<traits>()
                    .allow_algorithm(jwt::algorithm::rs256{pkey_itr->second})
                    .with_issuer(_issuer + '/')
                    .with_audience(_audience)
                    .verify(decoded_token)
                ;
                auto user_email = decoded_token.get_payload_claim("user_email").as_string();
                return user_email;
            } catch (nlohmann::json::exception& e) {
                std::cerr << e.what() << std::endl;
                res.code = crow::status::UNAUTHORIZED;
                res.body = "crow::AuthenticateMiddleware invalid json parse";
            } catch (const jwt::error::token_verification_exception& e) {
                std::cerr << e.what() << std::endl;
                res.code = crow::status::UNAUTHORIZED;
                res.body = "crow::AuthenticateMiddleware invalid Authorization token";
            } catch(std::runtime_error &e) {
                std::cerr << e.what() << std::endl;
                res.code = crow::status::INTERNAL_SERVER_ERROR;
                res.body = "crow::AuthenticateMiddleware server failed to parse token";
            } catch (...) {
                res.code = crow::status::INTERNAL_SERVER_ERROR;
                res.body = "crow::AuthenticateMiddleware server super failed to parse token";
            }
            res.end();
            return "";
        }

        void tryResolveUser(const std::string &user_email, crow::response &res, context &ctx) {
            auto user = _db->GetUser(user_email);
            if (user == nullptr) {
                res.code = crow::status::UNAUTHORIZED;
                res.body = "crow::AuthenticateMiddleware invalid user";
                res.end();
                return;
            }
            ctx.user = std::move(user);
        }

        const std::string _public_prefix = "/public/";

        const std::string _token_prefix = "Bearer ";
        infrastructure::DbPtr _db = nullptr;

        std::map<std::string, std::string> _public_keys;

        std::string _audience;
        std::string _issuer;
    };
}

#endif //INFRASTRUCTURE_WEBSERVER_AUTHENTICATE_MIDDLEWARE_HPP
