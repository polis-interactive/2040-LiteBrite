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


namespace infrastructure {

    struct WebServerConfig {
        /* this is a little scuffed; auth should be a middleware,
         *  and we should pass it a db instance so it can handle all auth ahead of time
         */
        utils::Duration auth_jwt_expiry;
        uint16_t web_server_port;
        int web_server_threads;

        static WebServerConfig from_json(const nlohmann::json& j) {
            WebServerConfig conf{};
            conf.auth_jwt_expiry = std::chrono::seconds(j.at("auth_jwt_expiry").get<unsigned int>());
            conf.web_server_port = j.at("web_server_port").get<uint16_t>();
            conf.web_server_threads = j.at("web_server_threads").get<int>();
            return conf;
        }
    };

    class WebServer;
    typedef std::shared_ptr<WebServer> WebServerPtr;

    struct WebServerManager {
        virtual std::pair<bool, std::string> TryLogin(const domain::User &user) = 0;
    };
    typedef std::shared_ptr<WebServerManager> WebServerManagerPtr;

    typedef crow::App<crow::CookieParser> CrowApp;

    class WebServer: public std::enable_shared_from_this<WebServer> {
    public:
        [[nodiscard]] static WebServerPtr Create(const WebServerConfig &config, WebServerManagerPtr manager);
        explicit WebServer(const WebServerConfig &conf, WebServerManagerPtr manager);
        void Start();
        void Stop();
        // no copy assignment, no empty assignment
        WebServer() = delete;
        WebServer (const WebServer&) = delete;
        WebServer& operator= (const WebServer&) = delete;
    private:
        const uint16_t _port;
        const int _threads;
        WebServerManagerPtr _manager;
        const utils::Duration _jwt_expiry;

        void initialize();
        void run();
        /* handlers */
        crow::response handleLogin(const crow::request& req);
        std::atomic<bool> _is_started = { false };
        std::unique_ptr<std::thread> _server_thread = nullptr;
        std::unique_ptr<CrowApp> _app = nullptr;
    };
}

#endif //INFRASTRUCTURE_WEBSERVER_HPP
