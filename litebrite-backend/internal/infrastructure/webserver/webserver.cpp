//
// Created by broosegoose on 7/9/23.
//

#include "webserver.hpp"

#include "options_route.hpp"


namespace infrastructure {

    WebServerPtr WebServer::Create(
        const WebServerConfig &conf, AuthPtr auth, DbPtr db, WebServerManagerPtr manager
    ) {
        auto server = std::make_shared<WebServer>(conf, std::move(auth), std::move(db), std::move(manager));
        server->initialize();
        return std::move(server);
    }

    WebServer::WebServer(
        const WebServerConfig &conf, AuthPtr auth, DbPtr db, WebServerManagerPtr manager
    ):
        _port(conf.web_server_port),
        _threads(conf.web_server_threads),
        _auth(std::move(auth)),
        _db(std::move(db)),
        _manager(std::move(manager)),
        _jwt_expiry(conf.auth_jwt_expiry),
        _is_dev(conf.web_server_dev)
    {}

    void WebServer::initialize() {
        std::cout << "infrastructure::Server(): initializing server" << std::endl;
        _app = std::make_unique<CrowApp>();
        _app->signal_clear();
        auto& cors = _app->get_middleware<crow::SubdomainCORSHandler>();
        cors
            .global()
                .methods("POST"_method, "GET"_method, "OPTIONS"_method)
            .prefix("/")
                .allow_credentials()
                .headers("Content-Type")
                .domainOrigin(_is_dev ? "localhost:3000" : "lighting.polis.tv")
        ;
        CROW_OPTIONS_ROUTE((*_app), "/api/auth/login")
            .methods("POST"_method)
            (std::bind_front(&WebServer::handleAuthLogin, shared_from_this()))
        ;
        CROW_OPTIONS_ROUTE((*_app), "/api/auth/identify")
            .methods("GET"_method)
            (std::bind_front(&WebServer::handleAuthIdentify, shared_from_this()))
        ;
        std::cout << "infrastructure::Server(): initialized!" << std::endl;

    }

    void WebServer::Start() {
        std::cout << "infrastructure::Server::Start(): starting server" << std::endl;
        if (_is_started) {
            std::cout << "infrastructure::Server::Start(): already started" << std::endl;
            return;
        }
        _is_started = true;
        auto self(shared_from_this());
        _server_thread = std::make_unique<std::thread>([this, self]() {
            run();
        });
        std::cout << "infrastructure::Server::Start(): started!" << std::endl;
    }

    void WebServer::Stop() {
        std::cout << "infrastructure::Server::Stop(): stopping server" << std::endl;
        if (!_is_started) {
            std::cout << "infrastructure::Server::Stop(): already stopped" << std::endl;
            return;
        }
        _is_started = false;
        if (_server_thread != nullptr) {
            if (_server_thread->joinable()) {
                _app->stop();
                _server_thread->join();
            }
            _server_thread.reset();
        }
        std::cout << "infrastructure::Server::Stop(): stopped!" << std::endl;

    }

    void WebServer::run() {
        std::cout << "infrastructure::Server::run(): running on port 8080 async" << std::endl;
        _app->
            port(_port)
            .concurrency(_threads)
            .run();
        std::cout << "infrastructure::Server::run(): server stopped" << std::endl;
    }
}