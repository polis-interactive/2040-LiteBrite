//
// Created by broosegoose on 7/18/23.
//

#ifndef INFRASTRUCTURE_ARTNET_HPP
#define INFRASTRUCTURE_ARTNET_HPP

#include <memory>

#include "domain/installation.hpp"

#include "controller.hpp"

namespace infrastructure {

    struct ArtNetConfig {
        domain::installation::Layout installation_layout;
        unsigned int artnet_buffer_count;
    };

    class ArtNet;
    typedef std::shared_ptr<ArtNet> ArtNetPtr;

    class ArtNet: public std::enable_shared_from_this<ArtNet> {
    public:
        static ArtNetPtr Create(const ArtNetConfig &config, net::io_context &context);
        ArtNet(const ArtNetConfig &config, net::io_context &context);
        void Start();
        void Post(utils::SizedBufferPtr &&art_net_pixels);
        void Stop();
        ~ArtNet();
        ArtNet() = delete;
        ArtNet (const ArtNet&) = delete;
        ArtNet& operator= (const ArtNet&) = delete;
    private:
        void run(std::stop_token st);
        void postPixels(utils::SizedBufferPtr &&art_net_pixels);
        std::atomic<bool> _work_started = { false };
        std::mutex _work_mutex;
        std::condition_variable _work_cv;
        // going to have to make this generic at some point, but for now eh
        std::queue<utils::SizedBufferPtr> _work_queue;
        std::unique_ptr<std::jthread> _work_thread = { nullptr };

        std::map<std::string, ArtNetControllerPtr> _controllers;
    };


}

#endif //INFRASTRUCTURE_ARTNET_HPP
