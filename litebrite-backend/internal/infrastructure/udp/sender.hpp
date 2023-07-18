//
// Created by broosegoose on 7/12/23.
//

#ifndef INFRASTRUCTURE_UDP_SENDER_HPP
#define INFRASTRUCTURE_UDP_SENDER_HPP

#include "utils/buffers.hpp"

#include "infrastructure/asio/context.hpp"

namespace infrastructure {

    struct UdpSenderConfig {
        std::string server_host;
        int server_port;
    };

    class UdpSender;
    typedef std::shared_ptr<UdpSender> UdpSenderPtr;

    class UdpSender: public std::enable_shared_from_this<UdpSender> {
    public:
        [[nodiscard]] static UdpSenderPtr Create(const UdpSenderConfig &config, net::io_context &context);
        UdpSender(const UdpSenderConfig &config, net::io_context &context);
        void Start();
        void Send(utils::SizedBufferPtr &&packet);
        void Stop();
        ~UdpSender();
        // no copy assignment, no empty assignment
        UdpSender() = delete;
        UdpSender (const UdpSender&) = delete;
        UdpSender& operator= (const UdpSender&) = delete;
    private:
        void doStart(bool is_initial_start);
        void onStart(error_code ec);
        void write(utils::SizedBufferPtr &&packet);
        /*
         * TODO: add timeout on write; but that might not be necessary b/c its udp? but it
         *  might be necessary if something is up with the os socket and we need a new one?
         */
        void doWrite();
        void onWrite(error_code ec, std::size_t bytes_transferred);
        void disconnect(bool reconnect);
        void doStop(std::promise<void> &&done_promise);
        std::atomic<bool> _is_stopped = {true};
        std::atomic<bool> _is_started = {false};
        const udp::endpoint _remote_endpoint;
        std::unique_ptr<udp::socket> _socket = {nullptr};
        net::strand<net::io_context::executor_type> _strand;
        std::queue<utils::SizedBufferPtr> _send_packet_queue = {};
        std::vector<char> _buffer;
    };
}

#endif //INFRASTRUCTURE_UDP_SENDER_HPP
