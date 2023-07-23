//
// Created by brucegoose on 3/5/23.
//

#ifndef INFRASTRUCTURE_ASIO_CONTEXT_HPP
#define INFRASTRUCTURE_ASIO_CONTEXT_HPP

#include <queue>
#include <memory>
#include <boost/asio.hpp>
#include <thread>
#include <iostream>

#include <nlohmann/json.hpp>

namespace net = boost::asio;
using boost::asio::ip::udp;
using boost::asio::ip::tcp;
using boost::system::error_code;

using boost::system::error_code;
typedef boost::asio::ip::address_v4 tcp_addr;

typedef net::detail::socket_option::boolean<SOL_SOCKET, SO_REUSEPORT> reuse_port;


inline void failOut(error_code ec, char const* what) {
    std::cerr << what << ": " << ec.message() << "\n";
    throw std::runtime_error(ec.message());
}

namespace infrastructure {

    struct AsioContextConfig {
        int asio_pool_size;
        static AsioContextConfig from_json(const nlohmann::json& j) {
            AsioContextConfig conf{};
            conf.asio_pool_size = j.at("asio_pool_size").get<int>();
            return conf;
        }
    };

    class AsioContext;
    typedef std::shared_ptr<AsioContext> AsioContextPtr;

    class AsioContext: public std::enable_shared_from_this<AsioContext> {
    public:

        static AsioContextPtr Create(const AsioContextConfig &config) {
            return std::make_shared<AsioContext>(config);
        }

        explicit AsioContext(const AsioContextConfig &config) :
                _context(config.asio_pool_size),
                _guard(net::make_work_guard(_context)),
                _pool(std::vector<std::jthread>(config.asio_pool_size))
        {}

        void Start() noexcept {
            if (_started) {
                return;
            }
            _started = true;
            std::generate(
                    _pool.begin(),
                    _pool.end(),
                    [&context = this->_context]()-> std::jthread {
                        return std::jthread([&context](std::stop_token st) {
                            while (!st.stop_requested()) {
                                try {
                                    context.run();
                                } catch (std::exception const &e) {
                                    std::cout << e.what() << std::endl;
                                } catch (...) {
                                    std::cout << "WTF Error" << std::endl;
                                }
                            }
                        });
                    }
            );
        }

        void Stop() {
            if (!_started) {
                return;
            }
            if (!_context.stopped()) {
                _guard.reset();
                _context.stop();
                if (!_pool.empty()) {
                    for (auto &thread : _pool) {
                        thread.request_stop();
                        thread.join();
                    }
                    _pool.clear();
                }
            }
            _started = false;
        }
        // should be marked friend and make this protected, or just use the member directly
        net::io_context &GetContext() {
            return _context;
        }
        ~AsioContext() {
            Stop();
        }
    private:
        net::io_context _context;
        net::executor_work_guard<net::io_context::executor_type> _guard;
        std::vector<std::jthread> _pool;
        std::atomic<bool> _started = { false };
    };

}


#endif //INFRASTRUCTURE_ASIO_CONTEXT_HPP
