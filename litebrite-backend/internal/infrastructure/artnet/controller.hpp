//
// Created by broosegoose on 7/18/23.
//

#ifndef INFRASTRUCTURE_ARTNET_CONTROLLER_HPP
#define INFRASTRUCTURE_ARTNET_CONTROLLER_HPP

#include <map>

#include "utils/buffers.hpp"

#include "domain/installation.hpp"

#include "infrastructure/udp/sender.hpp"

#include "packet.hpp"
#include "universe.hpp"


namespace infrastructure {

    struct ArtNetControllerConfig {
        std::string host;
        unsigned int buffer_count;
        std::map<unsigned int, domain::Universe> &universes;
    };

    class ArtNetController;

    typedef std::shared_ptr<ArtNetController> ArtNetControllerPtr;

    class ArtNetController {
    public:
        static ArtNetControllerPtr Create(const ArtNetControllerConfig &config, net::io_context &context);
        ArtNetController(const ArtNetControllerConfig &config, net::io_context &context);
        void Start();
        void Post(utils::SizedBufferPtr &artnet_pixels);
        void Stop();
        ~ArtNetController();
        // no copy assignment, no empty assignment
        ArtNetController() = delete;
        ArtNetController (const ArtNetController&) = delete;
        ArtNetController& operator= (const ArtNetController&) = delete;
    private:
        const std::string _host;
        std::map<unsigned int, ArtNetUniversePtr> _universes;
        std::shared_ptr<UdpSender> _sender;
    };

}

#endif //INFRASTRUCTURE_ARTNET_CONTROLLER_HPP
