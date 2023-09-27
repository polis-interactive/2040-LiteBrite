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
            auto site = _db->GetSite(user->site_id);
            j_res["sites"] = {site->to_json()};
        }
        sendJson(res, j_res);
    }

    void WebServer::handleSiteDisplayFetch(const crow::request &req, crow::response &res) {
        auto ctx = _app->get_context<crow::AuthorizeSiteHandler>(req);
        auto db_display = _db->GetDisplay(ctx.site->id);
        nlohmann::json j_res;
        if (db_display != nullptr) {
            j_res["display"] = db_display->to_json();
            j_res["is_default"] = false;
        } else {
            auto default_display = _manager->GetDefaultDisplay();
            j_res["display"] = default_display.to_json();
            j_res["is_default"] = true;
        }
        sendJson(res, j_res);
    }

    void WebServer::handleSiteDisplayUpdate(const crow::request &req, crow::response &res) {
        auto ctx = _app->get_context<crow::AuthorizeSiteHandler>(req);
        nlohmann::json j_res;
        j_res["success"] = true;
        try {
            const auto display = domain::Display::from_json(ctx.params["display"]);
            const auto success = _db->CreateOrUpdateDisplay(display, ctx.site->id);
            if (!success) {
                throw std::runtime_error("Failed to insert display into db");
            }
            _manager->SetCurrentDisplay(display);
            sendJson(res, j_res);
        } catch (nlohmann::json::exception& e) {
            std::cerr << e.what() << std::endl;
            res.code = crow::status::BAD_REQUEST;
            res.body = "WebServer::handleSiteDisplayUpdate invalid body payload";
        } catch(std::runtime_error &e) {
            std::cerr << e.what() << std::endl;
            res.code = crow::status::INTERNAL_SERVER_ERROR;
            res.body = "WebServer::handleSiteDisplayUpdate server failed to parse body";
        } catch (...) {
            res.code = crow::status::INTERNAL_SERVER_ERROR;
            res.body = "WebServer::handleSiteDisplayUpdate server super failed to do something";
        }
        res.end();
    }

    void WebServer::handleSiteDisplayDelete(const crow::request &req, crow::response &res) {
        auto ctx = _app->get_context<crow::AuthorizeSiteHandler>(req);
        const auto success = _db->DeleteDisplay(ctx.site->id);
        if (!success) {
            res.code = crow::status::BAD_REQUEST;
            res.body = "WebServer::handleSiteDisplayDelete nothing to delete...";
            res.end();
            return;
        }
        nlohmann::json j_res;
        auto default_display = _manager->ResetCurrentDisplay();
        j_res["display"] = default_display.to_json();
        sendJson(res, j_res);
    }


}