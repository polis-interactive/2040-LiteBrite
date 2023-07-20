//
// Created by broosegoose on 7/12/23.
//

#include "utils/clock.hpp"

#include "sender.hpp"

namespace infrastructure {
    UdpSenderPtr UdpSender::Create(const infrastructure::UdpSenderConfig &config, net::io_context &context) {
        auto sender = std::make_shared<UdpSender>(config, context);
        return std::move(sender);
    }

    UdpSender::UdpSender(const infrastructure::UdpSenderConfig &config, net::io_context &context):
        _strand(net::make_strand(context)),
        _remote_endpoint(
            net::ip::address::from_string(config.server_host),
            config.server_port
        )
    {}

    void UdpSender::Start() {
        if (!_is_stopped) return;
        _is_stopped = false;
        doStart(true);
    }

    void UdpSender::Stop() {
        if (_is_stopped) return;
        _is_stopped = true;

        std::promise<void> done_promise;
        auto done_future = done_promise.get_future();
        net::post(
            _strand,
            std::bind_front(&UdpSender::doStop, shared_from_this(), std::move(done_promise))
        );
        done_future.wait();
    }

    UdpSender::~UdpSender() {
        Stop();
    }

    void UdpSender::doStop(std::promise<void> &&done_promise) {
        disconnect(false);
        done_promise.set_value();
    }

    void UdpSender::doStart(bool is_initial_start) {
        if (_is_stopped) return;
        if (!is_initial_start) {
            std::this_thread::sleep_for(1s);
        }
        _socket = std::make_unique<udp::socket>(_strand);
        _socket->async_connect(_remote_endpoint, std::bind_front(&UdpSender::onStart, shared_from_this()));
    }

    void UdpSender::onStart(boost::system::error_code ec) {
        if (_is_stopped) return;
        if (ec) {
            std::cout << "UdpSender::onStart received err " << ec << " while connecting" <<std::endl;
            doStart(false);
            return;
        }
        _is_started = true;
    }

    void UdpSender::Send(utils::SizedBufferPtr &&packet) {
        if (_is_stopped || !_is_started) return;
        net::post(
            _strand,
            std::bind_front(&UdpSender::write, shared_from_this(), std::move(packet))
        );
    }

    void UdpSender::write(utils::SizedBufferPtr &&packet) {
        if (_is_stopped || !_is_started) return;
        _send_packet_queue.push(std::move(packet));
        if (_send_packet_queue.size() == 1) {
            doWrite();
        }
    }

    void UdpSender::doWrite() {
        const auto &packet_ptr = _send_packet_queue.front();
        _socket->async_send(
            net::buffer(packet_ptr->GetMemory(), packet_ptr->GetSize()),
            std::bind_front(&UdpSender::onWrite, shared_from_this())
        );
    }

    void UdpSender::onWrite(error_code ec, std::size_t bytes_transferred) {
        if (_is_stopped || !_is_started) return;
        const auto bytes_to_transfer = _send_packet_queue.front()->GetSize();
        if (!ec && bytes_transferred == bytes_to_transfer) {
            _send_packet_queue.pop();
            if (!_send_packet_queue.empty()) {
                doWrite();
            }
            return;
        }
        if (ec) {
            std::cout << "UdpSender::onWrite failed to with ec: " << ec << "; bailing" << std::endl;
            return;
        } else {
            std::cout << "UdpSender::onWrite only sent " << bytes_transferred << " of " << bytes_to_transfer <<
                " bytes; STAY IN THE MTU; bailing" << std::endl;
        }
        disconnect(true);
    }

    void UdpSender::disconnect(const bool reconnect) {
        _is_started = false;
        if (_socket->is_open()) {
            error_code ec;
            _socket->close(ec);
            if (ec) {
                std::cout << "UdpSender::disconnect failed to close with ec: " << ec << std::endl;
            }
        }
        _socket = nullptr;
        while(!_send_packet_queue.empty()) {
            _send_packet_queue.pop();
        }
        if (reconnect) {
            doStart(true);
        }
    }
}