//
// Created by broosegoose on 7/23/23.
//

#include "embedded_service.hpp"

#include <utility>

namespace service {

    EmbeddedServicePtr EmbeddedService::Create(const service::EmbeddedConfig &config) {
        auto embedded_service = std::make_shared<EmbeddedService>(config);
        embedded_service->initialize();
        return embedded_service;
    }

    EmbeddedService::EmbeddedService(service::EmbeddedConfig config): _config(std::move(config)), _is_started(false) {}

    void EmbeddedService::initialize() {
        _db = infrastructure::Db::Create(_config.db_config, shared_from_this());
        if (_db == nullptr) {
            throw std::runtime_error("Unable to startup db");
        }

        auto db_display = _db->GetDisplay(_config.site_id);

        _asio_context = infrastructure::AsioContext::Create(_config.asio_context_config);
        _art_net = infrastructure::ArtNet::Create(_config.art_net_config, _asio_context->GetContext());
        _graphics = infrastructure::Graphics::Create(_config.graphics_config, shared_from_this(), std::move(db_display));

        // web server goes last as it relies on other services
        _web_server = infrastructure::WebServer::Create(
            _config.web_server_config, _db, shared_from_this()
        );
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
        _db.reset();
    }

    void EmbeddedService::PostGraphicsUpdate(utils::SizedBufferPtr &&pixels) {
        _art_net->Post(std::move(pixels));
    }

    domain::Display EmbeddedService::GetDefaultDisplay() {
        return _config.graphics_config.default_display;
    }

    void EmbeddedService::SetCurrentDisplay(const domain::Display &display) {
        _graphics->SetDisplay(display);
    }

    domain::Display EmbeddedService::ResetCurrentDisplay() {
        _graphics->SetDisplay(_config.graphics_config.default_display);
        return _config.graphics_config.default_display;
    }

}