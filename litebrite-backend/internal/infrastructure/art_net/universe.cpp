//
// Created by broosegoose on 7/18/23.
//

#include <functional>
#include "universe.hpp"

namespace infrastructure {
    ArtNetUniversePtr ArtNetUniverse::Create(const ArtNetUniverseConfig &config) {
        auto universe = std::make_shared<ArtNetUniverse>(config);
        return std::move(universe);
    }

    ArtNetUniverse::ArtNetUniverse(const infrastructure::ArtNetUniverseConfig &config):
        _universe(config.universe),
        _buffer_count(config.buffer_count),
        _start_index(config.start_index),
        _length(config.length)
    {
        for (int i = 0; i < config.buffer_count; i++) {
            _packets.push(new ArtNedDmxPacket(_universe, _length));
        }
    }

    utils::SizedBufferPtr ArtNetUniverse::GetNextPacket(const utils::SizedBufferPtr &artnet_pixels) {
        ArtNedDmxPacket *packet = nullptr;
        {
            std::lock_guard<std::mutex> lk(_packet_lock);
            if (!_packets.empty()) {
                packet = _packets.front();
                _packets.pop();
            }
        }
        if (packet == nullptr) {
            return nullptr;
        }
        packet->UpdateSequence(++_sequence);
        packet->CopyBody(artnet_pixels->GetMemory() + _start_index);
        auto output_packet = std::shared_ptr<ArtNedDmxPacket>(
            packet, std::bind_front(&ArtNetUniverse::queuePacket, shared_from_this())
        );
        return std::move(output_packet);
    }

    void ArtNetUniverse::queuePacket(ArtNedDmxPacket *packet) {
        std::lock_guard<std::mutex> lk(_packet_lock);
        _packets.push(packet);
    }

    ArtNetUniverse::~ArtNetUniverse() {
        std::lock_guard<std::mutex> lk(_packet_lock);
        while (!_packets.empty()) {
            auto packet = _packets.front();
            _packets.pop();
            delete packet;
        }

    }

}