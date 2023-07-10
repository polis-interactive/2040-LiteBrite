//
// Created by broosegoose on 7/8/23.
//

#ifndef INFRASTRUCTURE_WEBSERVER_HPP
#define INFRASTRUCTURE_WEBSERVER_HPP

#include <memory>
#include <thread>
#include <atomic>

#include <crow.h>


namespace infrastructure {

    class Webserver: public std::enable_shared_from_this<Webserver> {
    public:
        static std::shared_ptr<Webserver> Create();
        Webserver() = default;
        void Start();
        void Stop();
    private:
        void initialize();
        void run();
        std::atomic<bool> _is_started = { false };
        std::unique_ptr<std::thread> _server_thread = nullptr;
        crow::SimpleApp _app;
    };
}

#endif //INFRASTRUCTURE_WEBSERVER_HPP
