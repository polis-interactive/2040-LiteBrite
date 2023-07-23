
#include "config.hpp"
#include "runtime.hpp"

#include "service/embedded_service.hpp"

int main(int argc, char* argv[]) {

    application::RemoveSuccessFile();
    auto json_config = application::get_json_config(application::AppType::EMBEDDED, argc, argv);
    auto service_config = service::EmbeddedConfig{
        .art_net_config = infrastructure::ArtNetConfig::from_json(json_config),
        .asio_context_config = infrastructure::AsioContextConfig::from_json(json_config),
        .graphics_config = infrastructure::GraphicsConfig::from_json(json_config),
    };
    auto service = service::EmbeddedService::Create(service_config);
    service->Start();
    application::WaitForShutdown();
    service->Stop();
    std::this_thread::sleep_for(250ms);
    service->Unset();
    std::this_thread::sleep_for(250ms);
    application::CreateSuccessFile();
    return 0;
}
