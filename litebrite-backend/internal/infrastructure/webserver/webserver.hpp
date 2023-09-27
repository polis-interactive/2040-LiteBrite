//
// Created by broosegoose on 7/8/23.
//

#ifndef INFRASTRUCTURE_WEBSERVER_HPP
#define INFRASTRUCTURE_WEBSERVER_HPP

#include <memory>
#include <thread>
#include <atomic>
#include <crow.h>
#include <crow/middlewares/cookie_parser.h>

#include "utils/clock.hpp"

#include "domain/user.hpp"
#include "domain/display.hpp"

#include "infrastructure/db/db.hpp"

#include "cors_options_middleware.hpp"
#include "authenticate_middleware.hpp"

#include "authorize_middleware.hpp"



namespace infrastructure {

    struct WebServerConfig {
        /* this is a little scuffed; auth should be a middleware,
         *  and we should pass it a db instance so it can handle all auth ahead of time
         */
        uint16_t web_server_port;
        int web_server_threads;
        bool web_server_dev;

        std::string web_server_auth_audience;
        std::string web_server_auth_issuer;

        static WebServerConfig from_json(const nlohmann::json& j) {
            WebServerConfig conf{};
            conf.web_server_port = j.at("web_server_port").get<uint16_t>();
            conf.web_server_threads = j.at("web_server_threads").get<int>();
            conf.web_server_dev = j.at("web_server_dev").get<bool>();
            conf.web_server_auth_audience = j.at("web_server_auth_audience").get<std::string>();
            conf.web_server_auth_issuer = j.at("web_server_auth_issuer").get<std::string>();
            return conf;
        }
    };

    class WebServer;
    typedef std::shared_ptr<WebServer> WebServerPtr;

    struct WebServerManager {
        virtual domain::Display GetDefaultDisplay() = 0;
        virtual void SetCurrentDisplay(const domain::Display &display) = 0;
        virtual domain::Display ResetCurrentDisplay() = 0;
    };
    typedef std::shared_ptr<WebServerManager> WebServerManagerPtr;

    typedef crow::App<
        crow::CORSOptionsHandler, crow::AuthenticateHandler, crow::AuthorizeSiteHandler,
        crow::AuthorizeAdminHandler
    > CrowApp;

    class WebServer: public std::enable_shared_from_this<WebServer> {
    public:
        [[nodiscard]] static WebServerPtr Create(
            const WebServerConfig &config, DbPtr db, WebServerManagerPtr manager
        );
        explicit WebServer(
            const WebServerConfig &conf, DbPtr db, WebServerManagerPtr manager
        );
        void Start();
        void Stop();
        // no copy assignment, no empty assignment
        WebServer() = delete;
        WebServer (const WebServer&) = delete;
        WebServer& operator= (const WebServer&) = delete;
    private:
        const uint16_t _port;
        const int _threads;
        DbPtr _db;
        WebServerManagerPtr _manager;
        const bool _is_dev;

        void initialize(const WebServerConfig &conf);
        void run();

        /* ROUTE HANDLERS */
        void handleIdentify(const crow::request& req, crow::response &res);
        // CRUD on display
        void handleSiteDisplayFetch(const crow::request& req, crow::response &res);
        void handleSiteDisplayUpdate(const crow::request& req, crow::response &res);
        void handleSiteDisplayDelete(const crow::request& req, crow::response &res);

        void sendJson(crow::response &res, const nlohmann::json& data);

        std::atomic<bool> _is_started = { false };
        std::unique_ptr<std::thread> _server_thread = nullptr;
        std::unique_ptr<CrowApp> _app = nullptr;
    };
}

#endif //INFRASTRUCTURE_WEBSERVER_HPP
