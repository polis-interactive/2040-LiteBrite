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

#include "infrastructure/auth/auth.hpp"
#include "infrastructure/db/db.hpp"

#include "subdomain_cors.hpp"


namespace infrastructure {

    struct WebServerConfig {
        /* this is a little scuffed; auth should be a middleware,
         *  and we should pass it a db instance so it can handle all auth ahead of time
         */
        utils::Duration auth_jwt_expiry;
        uint16_t web_server_port;
        int web_server_threads;
        bool web_server_dev;

        static WebServerConfig from_json(const nlohmann::json& j) {
            WebServerConfig conf{};
            conf.auth_jwt_expiry = std::chrono::seconds(j.at("auth_jwt_expiry").get<unsigned int>());
            conf.web_server_port = j.at("web_server_port").get<uint16_t>();
            conf.web_server_threads = j.at("web_server_threads").get<int>();
            conf.web_server_dev = j.at("web_server_dev").get<bool>();
            return conf;
        }
    };

    class WebServer;
    typedef std::shared_ptr<WebServer> WebServerPtr;

    struct WebServerManager {};
    typedef std::shared_ptr<WebServerManager> WebServerManagerPtr;

    typedef crow::App<crow::CookieParser, crow::SubdomainCORSHandler> CrowApp;

    class WebServer: public std::enable_shared_from_this<WebServer> {
    public:
        [[nodiscard]] static WebServerPtr Create(
            const WebServerConfig &config, AuthPtr auth, DbPtr db, WebServerManagerPtr manager
        );
        explicit WebServer(
            const WebServerConfig &conf, AuthPtr auth, DbPtr db, WebServerManagerPtr manager
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
        AuthPtr _auth;
        DbPtr _db;
        WebServerManagerPtr _manager;
        const utils::Duration _jwt_expiry;
        const bool _is_dev;

        void initialize();
        void run();

        /* auth */
        void authSetJwtCookie(const crow::request& req, const std::string &jwt);
        std::unique_ptr<domain::User> authVerifyIdentify(const crow::request &req);
        crow::response handleAuthLogin(const crow::request& req);
        crow::response handleAuthIdentify(const crow::request &req);


        std::atomic<bool> _is_started = { false };
        std::unique_ptr<std::thread> _server_thread = nullptr;
        std::unique_ptr<CrowApp> _app = nullptr;
    };
}

#endif //INFRASTRUCTURE_WEBSERVER_HPP
