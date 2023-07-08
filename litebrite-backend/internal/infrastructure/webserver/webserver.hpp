//
// Created by broosegoose on 7/8/23.
//

#ifndef INFRASTRUCTURE_DROGON_SERVER_HPP
#define INFRASTRUCTURE_DROGON_SERVER_HPP

#include <memory>
#include <thread>
#include <atomic>

#include <crow.h>

namespace infrastructure::webserver {

    class Webserver: public std::enable_shared_from_this<Webserver> {
    public:
        static std::shared_ptr<Webserver> Create() {
            auto server = std::make_shared<Webserver>();
            server->initialize();
            return std::move(server);
        }
        Webserver() = default;
        void Start() {
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
        void Stop() {
            std::cout << "infrastructure::Server::Stop(): stopping server" << std::endl;
            if (!_is_started) {
                std::cout << "infrastructure::Server::Stop(): already stopped" << std::endl;
                return;
            }
            _is_started = false;
            if (_server_thread != nullptr) {
                if (_server_thread->joinable()) {
                    _app.stop();
                    _server_thread->join();
                }
                _server_thread.reset();
            }
            std::cout << "infrastructure::Server::Stop(): stopped!" << std::endl;

        }
    private:
        void initialize() {
            std::cout << "infrastructure::Server(): initializing server" << std::endl;
            _app.signal_clear();
            CROW_ROUTE(_app, "/")([]() {
                return "Hello, melissa marianas trench!";
            });
            std::cout << "infrastructure::Server(): initialized!" << std::endl;
        }
        void run() {
            std::cout << "infrastructure::Server::run(): running on port 8080 async" << std::endl;
            _app.port(8080).concurrency(2).run();
            std::cout << "infrastructure::Server::run(): server stopped" << std::endl;
        }
        std::atomic_bool _is_started = false;
        std::unique_ptr<std::thread> _server_thread = nullptr;
        crow::SimpleApp _app;
    };
}

#endif //INFRASTRUCTURE_DROGON_SERVER_HPP
