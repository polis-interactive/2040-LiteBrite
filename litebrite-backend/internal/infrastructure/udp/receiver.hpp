//
// Created by broosegoose on 7/12/23.
//

#ifndef INFRASTRUCTURE_UDP_RECEIVER_HPP
#define INFRASTRUCTURE_UDP_RECEIVER_HPP

#include "infrastructure/asio/context.hpp"

namespace infrastructure {

    struct UdpReceiverConfig {
        const std::string bind_host;
        const std::string bind_port;
    };
}

#endif //INFRASTRUCTURE_UDP_RECEIVER_HPP
