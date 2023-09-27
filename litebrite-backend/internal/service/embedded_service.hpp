//
// Created by broosegoose on 7/23/23.
//

#ifndef SERVICE_EMBEDDED_HPP
#define SERVICE_EMBEDDED_HPP

#include "infrastructure/art_net/art_net.hpp"
#include "infrastructure/asio/context.hpp"
#include "infrastructure/db/db.hpp"
#include "infrastructure/graphics/graphics.hpp"
#include "infrastructure/webserver/webserver.hpp"

namespace service {

    struct EmbeddedConfig {
        infrastructure::ArtNetConfig art_net_config;
        infrastructure::AsioContextConfig asio_context_config;
        infrastructure::DbConfig db_config;
        infrastructure::GraphicsConfig graphics_config;
        int site_id;
        infrastructure::WebServerConfig web_server_config;
    };

    class EmbeddedService;
    typedef std::shared_ptr<EmbeddedService> EmbeddedServicePtr;

    class EmbeddedService:
        public infrastructure::DbManager,
        public infrastructure::GraphicsManager,
        public infrastructure::WebServerManager,
        public std::enable_shared_from_this<EmbeddedService>
    {
    public:
        [[nodiscard]] static EmbeddedServicePtr Create(const EmbeddedConfig &config);
        explicit EmbeddedService(EmbeddedConfig config);
        void Start();
        void Stop();
        void Unset();

        /* Manager Members */
        // graphics
        void PostGraphicsUpdate(utils::SizedBufferPtr &&pixels) final;

        // webserver
        domain::Display GetDefaultDisplay() final;
        void SetCurrentDisplay(const domain::Display &display) final;
        domain::Display ResetCurrentDisplay() final;

        // no copy assignment
        EmbeddedService (const EmbeddedService&) = delete;
        EmbeddedService& operator= (const EmbeddedService&) = delete;
    private:
        void initialize();
        const EmbeddedConfig _config;
        std::atomic_bool _is_started = false;
        infrastructure::ArtNetPtr _art_net;
        infrastructure::AsioContextPtr _asio_context;
        infrastructure::DbPtr _db;
        infrastructure::GraphicsPtr _graphics;
        infrastructure::WebServerPtr _web_server;
    };
}

#endif //SERVICE_EMBEDDED_HPP
