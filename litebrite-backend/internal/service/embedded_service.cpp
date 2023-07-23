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
        _graphics = infrastructure::Graphics::Create(config.graphics_config, shared_from_this());
        _asio_context = infrastructure::AsioContext::Create(config.asio_context_config);
        _art_net = infrastructure::ArtNet::Create(config.art_net_config, _asio_context->GetContext());
    }

    void EmbeddedService::Start() {
        if (_is_started) {
            return;
        }
        _asio_context->Start();
        _art_net->Start();
        _graphics->Start();
        _is_started = true;
    }

    void EmbeddedService::Stop() {
        if (!_is_started) {
            return;
        }
        _graphics->Stop();
        _art_net->Stop();
        _asio_context->Stop();
        _is_started = false;
    }

    void EmbeddedService::Unset() {
        _graphics.reset();
        _art_net.reset();
        _asio_context.reset();
    }

    void EmbeddedService::PostGraphicsUpdate(utils::SizedBufferPtr &&pixels) {
        _art_net->Post(std::move(pixels));
    }


}