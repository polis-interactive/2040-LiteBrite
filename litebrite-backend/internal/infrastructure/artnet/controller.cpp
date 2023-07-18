//
// Created by broosegoose on 7/18/23.
//

#include "controller.hpp"

namespace infrastructure {
    ArtNetControllerPtr ArtNetController::Create(
        const infrastructure::ArtNetControllerConfig &config, net::io_context &context)
    {
        return std::make_shared<ArtNetController>(config, context);
    }

    ArtNetController::ArtNetController(
        const infrastructure::ArtNetControllerConfig &config, net::io_context &context
    ): _host(config.host) {
        _sender = UdpSender::Create(
            {
                .server_host = _host,
                .server_port = ARTNET_PORT
            }, context
        );
        for (const auto &[universe_number, universe_mapping]: config.universes) {
            _universes.insert({
                universe_number,
                ArtNetUniverse::Create({
                    .start_pixel = universe_mapping.start_position,
                    .length = universe_mapping.pixel_count * universe_mapping.pixel_width,
                    .univserse = universe_number,
                    .buffer_count = config.buffer_count
                })
            });
        }
    }

    void ArtNetController::Start() {
        _sender->Start();
    }

    void ArtNetController::Send(utils::SizedBufferPtr &artnet_pixels) {
        for (auto &[_, universe_ptr]: _universes) {
            auto next_universe_packet = universe_ptr->GetNextPacket(artnet_pixels);
            if (next_universe_packet == nullptr) {
                continue;
            }
            _sender->Send(std::move(next_universe_packet));
        }
    }

    void ArtNetController::Stop() {
        _sender->Stop();
    }
}