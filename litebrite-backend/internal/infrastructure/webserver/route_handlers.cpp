//
// Created by broosegoose on 9/9/23.
//

#include "./webserver.hpp"

namespace infrastructure {
    void WebServer::handleIdentify(const crow::request &req, crow::response &res) {
        auto &user = _app->get_context<crow::AuthenticateHandler>(req).user;
        nlohmann::json j_res;
        j_res["user"] = user->to_application_json();
        if (user->is_admin) {
            nlohmann::json sites = _db->GetAllSites();
            j_res["sites"] = sites;
        } else {
            auto site = _db->GetSite(user->id);
            j_res["sites"] = {site->to_json()};
        }
        sendJson(res, j_res);
    }
}