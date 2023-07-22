//
// Created by broosegoose on 7/19/23.
//

#include <doctest.h>
#include <thread>
#include <optional>
#include <iostream>

#include "utils/buffers.hpp"
#include "utils/clock.hpp"

#include "domain/installation.hpp"

#include "infrastructure/artnet/artnet.hpp"

#include "../test_asio_utils.hpp"

[[nodiscard]] infrastructure::ArtNetConfig GetArtNetControllerConfig(
    std::optional<std::string> installation_string
) {
    auto installation_json = nlohmann::json::parse(
        installation_string.has_value()
        ? installation_string.value()
        : R"({
            "universes": {
                "0": { "pixel_count": 5, "pixel_width": 3, "start_index": 0 },
                "1": { "pixel_count": 5, "pixel_width": 4, "start_index": 15 },
                "2": { "pixel_count": 5, "pixel_width": 3, "start_index": 35 }
            },
            "controllers": {
                "127.0.0.1": [0, 1, 2]
            }
        })"
    );
    auto layout = domain::installation::Layout::from_json(installation_json);
    return {
        .installation_layout = layout,
        .artnet_buffer_count = 3
    };
}

TEST_CASE("Infrastructure_ArtNet-Creation") {
    auto ctx = GetAsioTestContext({});
    const auto conf = GetArtNetControllerConfig({});
    auto art_net = infrastructure::ArtNet::Create(conf, ctx->GetContext());
    REQUIRE((bool)(art_net != nullptr));
}

TEST_CASE("Infrastructure_ArtNet-StartAndStop") {
    auto ctx = GetAsioTestContext({});
    const auto conf = GetArtNetControllerConfig({});
    auto art_net = infrastructure::ArtNet::Create(conf, ctx->GetContext());
    ctx->Start();
    art_net->Start();
    // give the network time to initialize
    std::this_thread::sleep_for(1ms);
    art_net->Stop();
    ctx->Stop();
}

TEST_CASE("Infrastructure_ArtNet-SendPacket") {
    auto ctx = GetAsioTestContext({});
    const auto conf = GetArtNetControllerConfig({});
    auto art_net = infrastructure::ArtNet::Create(conf, ctx->GetContext());
    ctx->Start();
    art_net->Start();
    // give the network / artnet loop to initialize
    std::this_thread::sleep_for(10ms);

    auto buffer = new utils::VectorSizedBuffer(50);
    std::iota (std::begin(buffer->_buffer), std::end(buffer->_buffer), 1);
    std::promise<void> done_promise;
    auto done_future = done_promise.get_future();
    auto buffer_ptr = std::shared_ptr<utils::VectorSizedBuffer>(
            buffer, [p = std::move(done_promise)](utils::VectorSizedBuffer *ptr) mutable {
                p.set_value();
            }
    );
    art_net->Post(std::move(buffer_ptr));
    REQUIRE_NE(done_future.wait_for(10ms), std::future_status::timeout);
    // at this point in time, we don't really know if udp successfully sent packets as we don't have a receiver
    // written; using `ncat -u -l 6454` and wireshark for testing, and it works!
    std::this_thread::sleep_for(500ms);
    art_net->Stop();
    ctx->Stop();
}