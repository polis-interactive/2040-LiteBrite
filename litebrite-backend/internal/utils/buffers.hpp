//
// Created by broosegoose on 7/17/23.
//

#ifndef UTILS_BUFFERS_HPP
#define UTILS_BUFFERS_HPP

#include <memory>

namespace utils {

    struct SizedBuffer {
        [[nodiscard]] virtual uint8_t *GetMemory() = 0;
        [[nodiscard]] virtual std::size_t GetSize() = 0;
    };
    typedef std::shared_ptr<SizedBuffer> SizedBufferPtr;

}

#endif //UTILS_BUFFERS_HPP
