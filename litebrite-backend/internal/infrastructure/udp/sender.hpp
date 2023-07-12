//
// Created by broosegoose on 7/12/23.
//

#ifndef INFRASTRUCTURE_UDP_SENDER_HPP
#define INFRASTRUCTURE_UDP_SENDER_HPP

#include "infrastructure/asio/context.hpp"

namespace infrastructure {

    struct UdpClientConfig {
        const std::string server_host;
        const std::string server_port;
    };
}

#endif //INFRASTRUCTURE_UDP_CLIENT_HPP
