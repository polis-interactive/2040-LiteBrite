//
// Created by broosegoose on 7/9/23.
//

#include "webserver.hpp"

#include "options_route.hpp"


namespace infrastructure {

    WebServerPtr WebServer::Create(
        const WebServerConfig &conf, DbPtr db, WebServerManagerPtr manager
    ) {
        auto server = std::make_shared<WebServer>(conf, std::move(db), std::move(manager));
        server->initialize(conf);
        return std::move(server);
    }

    WebServer::WebServer(
        const WebServerConfig &conf, DbPtr db, WebServerManagerPtr manager
    ):
        _port(conf.web_server_port),
        _threads(conf.web_server_threads),
        _db(std::move(db)),
        _manager(std::move(manager)),
        _is_dev(conf.web_server_dev)
    {}

    void WebServer::initialize(const WebServerConfig &conf) {
        std::cout << "infrastructure::Server(): initializing server" << std::endl;
        // initialize the app
        _app = std::make_unique<CrowApp>();
        // don't allow cntrl-c to unwrap crow
        _app->signal_clear();

        // setup cors
        auto &cors = _app->get_middleware<crow::CORSOptionsHandler>();
        cors
            .global()
                .methods("POST"_method, "GET"_method)
            .prefix("/")
                .allow_credentials()
                .headers("Content-Type, Authorization")
                .origin(_is_dev ? "http://localhost:3000" : "lighting.polis.tv")
        ;

        // setup authentication
        auto &auth_n = _app->get_middleware<crow::AuthenticateHandler>();
        auth_n
            .SetDb(_db)
            .SetAudience(conf.web_server_auth_audience)
            .SetIssuer(conf.web_server_auth_issuer)
        ;

        // setup authorization
        auto &auth_z_site = _app->get_middleware<crow::AuthorizeSiteHandler>();
        auth_z_site.SetDb(_db);

        // setup routes
        CROW_OPTIONS_ROUTE((*_app), "/ping")
            .methods("POST"_method)
            ([&](const crow::request& req, crow::response &res){
                nlohmann::json j_res = "PONG_PUBLIC";
                return sendJson(res, j_res);
            })
        ;

        CROW_OPTIONS_ROUTE((*_app), "/identify")
            .methods("POST"_method)
            (std::bind_front(&WebServer::handleIdentify, shared_from_this()))
        ;

        // setup site routes

        CROW_OPTIONS_ROUTE((*_app), "/site/ping")
            .methods("POST"_method)
            ([&](const crow::request& req, crow::response &res){
                nlohmann::json j_res = "PONG_SITE";
                return sendJson(res, j_res);
            })
        ;

        // setup admin routes

        CROW_OPTIONS_ROUTE((*_app), "/admin/ping")
            .methods("POST"_method)
            ([&](const crow::request& req, crow::response &res){
                nlohmann::json j_res = "PONG_ADMIN";
                return sendJson(res, j_res);
            })
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
        std::cout << "infrastructure::Server::run(): running on port " << _port << " async" << std::endl;
        _app->
            port(_port)
            .concurrency(_threads)
            .run();
        std::cout << "infrastructure::Server::run(): server stopped" << std::endl;
    }

    void WebServer::sendJson(crow::response &res, const nlohmann::json &data) {
        res.add_header("Access-Control-Allow-Origin", "*");
        res.add_header("Content-Type", "application/json");
        res.write(data.dump());
        res.end();
    }
}