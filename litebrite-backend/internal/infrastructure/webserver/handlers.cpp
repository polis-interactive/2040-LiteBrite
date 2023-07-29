//
// Created by broosegoose on 7/29/23.
//

#include "webserver.hpp"

#include <nlohmann/json.hpp>

namespace infrastructure {

    crow::response WebServer::handleLogin(const crow::request &req) {
        try {
            const auto login_payload = nlohmann::json::parse(req.body);
            const auto user = domain::User::from_json(login_payload);
            const auto [success, jwt] = _manager->TryLogin(user);
            if (!success) {
                return { 401, "Bad Login" };
            } else {
                auto &ctx = _app->get_context<crow::CookieParser>(req);
                ctx.set_cookie("lighting-session", jwt).path("/api").max_age(_jwt_expiry.count());
                return crow::response(200);
            }
        } catch (nlohmann::json::exception& e) {
            return { 400, "Bad JSON payload" };
        }
    }

}