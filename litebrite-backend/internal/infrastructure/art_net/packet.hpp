//
// Created by broosegoose on 7/18/23.
//

#ifndef INFRASTRUCTURE_ARTNET_PACKET_HPP
#define INFRASTRUCTURE_ARTNET_PACKET_HPP

#include <cstring>

#include "utils/buffers.hpp"

#define ARTNET_PORT 6454

namespace infrastructure {
    /* I hearby declare that I will eventually implement the full artnet standard */

    class ArtNedDmxPacket: public utils::SizedBuffer {
    public:
        ArtNedDmxPacket(unsigned int universe, unsigned int length):
            _universe_high((universe >> 8) & 0xFF),
            _universe_low(universe & 0xFF),
            _length_high((length >> 8) & 0xFF),
            _length_low(length & 0xFF),
            _length(length),
            _data(std::vector<uint8_t>(length + 18))
        {
            std::memcpy(_data.data(), &_header, 18);
        }
        void UpdateSequence(const uint8_t sequence) {
            _data.at(12) = sequence;
        }
        // MAN THIS IS UNSAFE :D
        void CopyBody(uint8_t *body) {
            std::memcpy(_data.data() + 18, body, _length);
        }
        [[nodiscard]] uint8_t *GetMemory() final {
            return _data.data();
        }
        [[nodiscard]] std::size_t GetSize() final {
            return _data.size();
        };
    private:
        union {
            struct {
                uint8_t _id[8] = "Art-Net";
                uint16_t _op_code = 0x5000;
                uint16_t _version = 0x0e00;
                uint8_t _sequence = 0;
                uint8_t _physical = 0;
                uint8_t _universe_low;
                uint8_t _universe_high;
                uint8_t _length_high;
                uint8_t _length_low;
            };
            uint8_t _header[18];
        };
        const unsigned int _length;
        std::vector<uint8_t> _data;
    };
}

#endif //INFRASTRUCTURE_ARTNET_PACKET_HPP
