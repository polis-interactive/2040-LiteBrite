//
// Created by broosegoose on 7/9/23.
//

#include "webserver.hpp"

namespace infrastructure {

    std::shared_ptr<Webserver> Webserver::Create() {
        auto server = std::make_shared<Webserver>();
        server->initialize();
        return std::move(server);
    }

    void Webserver::initialize() {
        std::cout << "infrastructure::Server(): initializing server" << std::endl;
        _app = std::make_unique<crow::SimpleApp>();
        _app->signal_clear();
        CROW_ROUTE((*_app), "/")([]() {
            return "fk the world";
        });
        std::cout << "infrastructure::Server(): initialized!" << std::endl;

    }

    void Webserver::Start() {
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

    void Webserver::Stop() {
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

    void Webserver::run() {
        std::cout << "infrastructure::Server::run(): running on port 8080 async" << std::endl;
        _app->
            port(8080)
            .use_compression(crow::compression::algorithm::GZIP)
            .concurrency(4)
            .run();
        std::cout << "infrastructure::Server::run(): server stopped" << std::endl;
    }
}