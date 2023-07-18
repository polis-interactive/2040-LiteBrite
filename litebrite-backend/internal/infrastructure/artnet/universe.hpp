//
// Created by broosegoose on 7/18/23.
//

#ifndef INFRASTRUCTURE_UNIVERSE_HPP
#define INFRASTRUCTURE_UNIVERSE_HPP

namespace infrastructure {

    struct ArtNetUniverseConfig {
        unsigned int start_pixel;
        unsigned int length;
        unsigned int univserse;
        unsigned int buffer_count;
    };

    class ArtNetUniverse;
    typedef std::shared_ptr<ArtNetUniverse> ArtNetUniversePtr;

    class ArtNetUniverse: std::enable_shared_from_this<ArtNetUniverse> {
    public:
        static ArtNetUniversePtr Create(const ArtNetUniverseConfig &config);
        ArtNetUniverse(const ArtNetUniverseConfig &config);
        utils::SizedBufferPtr &&GetNextPacket(const utils::SizedBufferPtr &artnet_pixels);
    private:

    };
}

#endif //INFRASTRUCTURE_UNIVERSE_HPP
