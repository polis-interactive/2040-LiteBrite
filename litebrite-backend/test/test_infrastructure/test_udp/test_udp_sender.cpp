//
// Created by broosegoose on 7/19/23.
//


#include <doctest.h>
#include <thread>
#include <optional>
#include <iostream>

#include "utils/buffers.hpp"
#include "utils/clock.hpp"

#include "infrastructure/udp/sender.hpp"

#include "../test_asio_utils.hpp"

[[nodiscard]] infrastructure::UdpSenderConfig GetUdpSenderTestConfig(
        std::optional<std::string> host,
        std::optional<int> port
) {
    return {
        .server_host = host.has_value() ? host.value() : "127.0.0.1",
        .server_port = port.has_value() ? port.value() : 6969
    };
}

TEST_CASE("Infrastructure_UdpSender-Creation") {
    auto ctx = GetAsioTestContext({});
    const auto conf = GetUdpSenderTestConfig({}, {});
    auto sender = infrastructure::UdpSender::Create(conf, ctx->GetContext());
    REQUIRE((bool)(sender != nullptr));
}

TEST_CASE("Infrastructure_UdpSender-StartAndStop") {
    auto ctx = GetAsioTestContext({});
    const auto conf = GetUdpSenderTestConfig({}, {});
    auto sender = infrastructure::UdpSender::Create(conf, ctx->GetContext());
    ctx->Start();
    sender->Start();
    // give the network time to initialize
    std::this_thread::sleep_for(1ms);
    REQUIRE(sender->CheckIsStarted());
    sender->Stop();
    ctx->Stop();
}


TEST_CASE("Infrastructure_UdpSender-SendAPacket") {
    auto ctx = GetAsioTestContext({});
    const auto conf = GetUdpSenderTestConfig({}, {});
    auto sender = infrastructure::UdpSender::Create(conf, ctx->GetContext());
    ctx->Start();
    sender->Start();
    // give the network time to initialize
    std::this_thread::sleep_for(1ms);
    REQUIRE(sender->CheckIsStarted());
    auto buffer = new utils::StringSizedBuffer("Hello World!");
    std::promise<void> done_promise;
    auto done_future = done_promise.get_future();
    auto buffer_ptr = std::shared_ptr<utils::StringSizedBuffer>(
        buffer, [p = std::move(done_promise)](utils::StringSizedBuffer *ptr) mutable {
            p.set_value();
        }
    );
    sender->Send(std::move(buffer_ptr));
    REQUIRE_NE(done_future.wait_for(1ms), std::future_status::timeout);
    sender->Stop();
    ctx->Stop();
}