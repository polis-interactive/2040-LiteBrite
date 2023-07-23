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

    struct WebServerConfig {

    };

    class WebServer;
    typedef std::shared_ptr<WebServer> WebServerPtr;

    class WebServer: public std::enable_shared_from_this<WebServer> {
    public:
        [[nodiscard]] static WebServerPtr Create(const WebServerConfig &config);
        explicit WebServer(const WebServerConfig &conf);
        void Start();
        void Stop();
        // no copy assignment, no empty assignment
        WebServer() = delete;
        WebServer (const WebServer&) = delete;
        WebServer& operator= (const WebServer&) = delete;
    private:
        void initialize();
        void run();
        std::atomic<bool> _is_started = { false };
        std::unique_ptr<std::thread> _server_thread = nullptr;
        crow::SimpleApp _app;
    };
}

#endif //INFRASTRUCTURE_WEBSERVER_HPP
