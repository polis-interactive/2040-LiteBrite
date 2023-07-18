//
// Created by broosegoose on 7/17/23.
//

#ifndef UTILS_BUFFERS_HPP
#define UTILS_BUFFERS_HPP

#include <memory>

namespace utils {

    struct SizedBuffer {
        [[nodiscard]] virtual void *GetMemory() = 0;
        [[nodiscard]] virtual std::size_t GetSize() = 0;
    };
    typedef std::shared_ptr<SizedBuffer> SizedBufferPtr;

    struct SizedBufferPacket;

    typedef std::unique_ptr<SizedBufferPacket> SizedBufferPacketPtr;

    struct SizedBufferPacket {
        [[nodiscard]] static SizedBufferPacketPtr Create(SizedBufferPtr &&header, SizedBufferPtr &&body) {
            return std::make_unique<SizedBufferPacket>(std::move(header), std::move(body));
        }
        SizedBufferPacket(SizedBufferPtr &&header, SizedBufferPtr &&body):
            _header(std::move(header)),
            _body(std::move(body))
        {}
        [[nodiscard]] std::size_t GetSize() {
            return _header->GetSize() + _body->GetSize();
        }
        // no copy assignment, no empty assignment; really need a "mixin" for this
        SizedBufferPacket() = delete;
        SizedBufferPacket (const SizedBufferPacket&) = delete;
        SizedBufferPacket& operator= (const SizedBufferPacket&) = delete;
        SizedBufferPtr _header;
        SizedBufferPtr _body;
    };
}

#endif //UTILS_BUFFERS_HPP
