//
// Created by broosegoose on 7/29/23.
//

#include "webserver.hpp"

#include <nlohmann/json.hpp>

namespace infrastructure {

    crow::response WebServer::handleAuthLogin(const crow::request &req) {
        try {
            const auto login_payload = nlohmann::json::parse(req.body);
            const auto user = domain::User::from_json(login_payload);
            auto db_user = _db->GetUser(user.email);
            if (db_user == nullptr) {
                return { crow::status::UNAUTHORIZED, "Bad Login" };
            }
            auto [success, jwt] = _auth->VerifyLogin(user, (*db_user));
            if (!success) {
                return { crow::status::UNAUTHORIZED, "Bad Login" };
            }
            authSetJwtCookie(req, jwt);
            return {crow::status::OK, to_string(db_user->to_json())};
        } catch (nlohmann::json::exception& e) {
            return { crow::status::BAD_REQUEST, "Bad JSON Payload" };
        }
    }

    crow::response WebServer::handleAuthIdentify(const crow::request &req) {
        auto user = authVerifyIdentify(req);
        if (user == nullptr) {
            return { crow::status::UNAUTHORIZED, "No Credentials" };
        }
        return {crow::status::OK, to_string(user->to_json())};
    }

    void WebServer::authSetJwtCookie(const crow::request &req, const std::string &jwt) {
        auto &ctx = _app->get_context<crow::CookieParser>(req);
        auto &cookie = ctx.set_cookie("lighting-session", jwt);
        if (!jwt.empty()) {
            cookie
                .path("/")
                .max_age(_jwt_expiry.count())
                .same_site(
                    _is_dev
                        ? crow::CookieParser::Cookie::SameSitePolicy::None
                        : crow::CookieParser::Cookie::SameSitePolicy::Strict
                )
                .secure()
                .httponly()
            ;
        }
    }

    std::unique_ptr<domain::User> WebServer::authVerifyIdentify(const crow::request &req) {
        auto &ctx = _app->get_context<crow::CookieParser>(req);
        auto jwt = ctx.get_cookie("lighting-session");
        auto [success, new_jwt, user_id] = _auth->ValidateToken(jwt);
        if (!success) {
            return nullptr;
        }
        auto db_user =  _db->GetUser(user_id);
        if (db_user == nullptr) {
            _auth->RemoveJwt(new_jwt.empty() ? jwt : new_jwt);
            authSetJwtCookie(req, "");
            return nullptr;
        }
        if (!new_jwt.empty()) {
            authSetJwtCookie(req, new_jwt);
        }
        return std::move(db_user);
    }

}