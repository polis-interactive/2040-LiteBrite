//
// Created by broosegoose on 7/23/23.
//

#include "embedded_service.hpp"

namespace service {

    EmbeddedServicePtr EmbeddedService::Create(const service::EmbeddedConfig &config) {
        auto embedded_service = std::make_shared<EmbeddedService>();
        embedded_service->initialize(config);
        return embedded_service;
    }

    EmbeddedService::EmbeddedService(): _is_started(false) {}

    void EmbeddedService::initialize(const service::EmbeddedConfig &config) {
        _asio_context = infrastructure::AsioContext::Create(config.asio_context_config);
        _art_net = infrastructure::ArtNet::Create(config.art_net_config, _asio_context->GetContext());
        _graphics = infrastructure::Graphics::Create(config.graphics_config, shared_from_this());
        _auth = infrastructure::Auth::Create(config.auth_config);
        // db must go after auth (it uses auth to hash passwords at startup)
        _db = infrastructure::Db::Create(config.db_config, shared_from_this());
        if (_db == nullptr) {
            throw std::runtime_error("Unable to startup db");
        }
        // web server goes last as it relies on other services
        _web_server = infrastructure::WebServer::Create(config.web_server_config, shared_from_this());
    }

    void EmbeddedService::Start() {
        if (_is_started) {
            return;
        }
        _asio_context->Start();
        _art_net->Start();
        _graphics->Start();
        _web_server->Start();
        _is_started = true;
    }

    void EmbeddedService::Stop() {
        if (!_is_started) {
            return;
        }
        _web_server->Stop();
        _graphics->Stop();
        _art_net->Stop();
        _asio_context->Stop();
        _is_started = false;
    }

    void EmbeddedService::Unset() {
        _graphics.reset();
        _art_net.reset();
        _asio_context.reset();
        _web_server.reset();
        _auth.reset();
        _db.reset();
    }

    bool EmbeddedService::HashPassword(domain::User &user) {
        return _auth->HashPassword(user);
    }

    void EmbeddedService::PostGraphicsUpdate(utils::SizedBufferPtr &&pixels) {
        _art_net->Post(std::move(pixels));
    }

    std::pair<bool, std::string> EmbeddedService::TryLogin(const domain::User &user) {
        auto db_user = _db->GetUser(user.email);
        if (db_user == nullptr) {
            return { false, "" };
        }
        return _auth->VerifyLogin(user, (*db_user));
    }


}