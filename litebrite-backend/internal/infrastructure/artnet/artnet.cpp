//
// Created by broosegoose on 7/18/23.
//

#include "artnet.hpp"

namespace infrastructure {
    ArtNetPtr ArtNet::Create(const infrastructure::ArtNetConfig &config, net::io_context &context) {
        auto art_net = std::make_shared<ArtNet>(config, context);
        return std::move(art_net);
    }

    ArtNet::ArtNet(const infrastructure::ArtNetConfig &config, net::io_context &context) {
        const auto &layout = config.installation_layout;
        for (const auto &[host, universes]: layout.controllers) {
            std::map<unsigned int, domain::Universe> controller_universes;
            for (const auto &universe : universes) {
                auto it = layout.universes.find(universe);
                if (it != layout.universes.end()) {
                    controller_universes[universe] = it->second;
                }
            }
            _controllers.insert({
                host, ArtNetController::Create({
                    .host = host,
                    .buffer_count = config.artnet_buffer_count,
                    .universes = controller_universes
                }, context)
            });
        }
    }

    ArtNet::~ArtNet() {
        Stop();
    }

    void ArtNet::Start() {
        if (_work_thread != nullptr) {
            return;
        }

        for (auto &[_, controller]: _controllers) {
            controller->Start();
        }
        _work_thread = std::make_unique<std::jthread>(std::bind_front(&ArtNet::run, shared_from_this()));
    }

    void ArtNet::Stop() {
        if (_work_thread == nullptr) {
            return;
        }
        if (_work_thread->joinable()) {
            _work_thread->request_stop();
            _work_thread->join();
        }
        _work_thread = nullptr;
        for (auto &[_, controller]: _controllers) {
            controller->Stop();
        }
    }

    void ArtNet::Post(utils::SizedBufferPtr &&art_net_pixels) {
        if (!_work_started) {
            return;
        }
        std::unique_lock<std::mutex> lock(_work_mutex);
        _work_queue.push(std::move(art_net_pixels));
        _work_cv.notify_one();
    }

    void ArtNet::run(std::stop_token st) {
        std::stop_callback cb(st, [this]() {
            _work_cv.notify_all();
        });
        _work_started = true;
        while (!st.stop_requested()) {
            utils::SizedBufferPtr art_net_pixels;
            {
                std::unique_lock<std::mutex> lock(_work_mutex);
                _work_cv.wait(lock, [st, this]() {
                    return st.stop_requested() || !_work_queue.empty();
                });
                if (st.stop_requested()) {
                    break;
                } else if (_work_queue.empty()) {
                    continue;
                }
                art_net_pixels = _work_queue.front();
                _work_queue.pop();
            }
            postPixels(std::move(art_net_pixels));
        }
        _work_started = false;
    }

    void ArtNet::postPixels(utils::SizedBufferPtr &&art_net_pixels) {
        for (auto &[_, controller]: _controllers) {
            controller->Post(art_net_pixels);
        }
    }
}
