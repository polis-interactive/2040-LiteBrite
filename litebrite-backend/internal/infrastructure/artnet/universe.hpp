//
// Created by broosegoose on 7/18/23.
//

#ifndef INFRASTRUCTURE_UNIVERSE_HPP
#define INFRASTRUCTURE_UNIVERSE_HPP

#include <memory>
#include <queue>
#include <atomic>

#include "packet.hpp"

namespace infrastructure {

    struct ArtNetUniverseConfig {
        unsigned int start_index;
        unsigned int length;
        unsigned int universe;
        unsigned int buffer_count;
    };

    class ArtNetUniverse;
    typedef std::shared_ptr<ArtNetUniverse> ArtNetUniversePtr;

    class ArtNetUniverse: public std::enable_shared_from_this<ArtNetUniverse> {
    public:
        static ArtNetUniversePtr Create(const ArtNetUniverseConfig &config);
        ArtNetUniverse(const ArtNetUniverseConfig &config);
        utils::SizedBufferPtr GetNextPacket(const utils::SizedBufferPtr &artnet_pixels);
        ~ArtNetUniverse();
        // no copy assignment, no empty assignment
        ArtNetUniverse() = delete;
        ArtNetUniverse (const ArtNetUniverse&) = delete;
        ArtNetUniverse& operator= (const ArtNetUniverse&) = delete;
    private:
        void queuePacket(ArtNedDmxPacket *packet);
        const unsigned int _start_index;
        const unsigned int _length;
        // I technically don't use these, but the don't hurt anyone
        const unsigned int _buffer_count;
        const unsigned int _universe;
        uint8_t _sequence = 0;
        std::mutex _packet_lock;
        std::queue<ArtNedDmxPacket *> _packets;
    };
}

#endif //INFRASTRUCTURE_UNIVERSE_HPP
