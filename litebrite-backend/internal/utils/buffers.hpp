//
// Created by broosegoose on 7/17/23.
//

#ifndef UTILS_BUFFERS_HPP
#define UTILS_BUFFERS_HPP

#include <memory>
#include <vector>


namespace utils {

    struct SizedBuffer {
        [[nodiscard]] virtual uint8_t *GetMemory() = 0;
        [[nodiscard]] virtual std::size_t GetSize() = 0;
    };
    typedef std::shared_ptr<SizedBuffer> SizedBufferPtr;

    struct StringSizedBuffer: public SizedBuffer {
        explicit StringSizedBuffer(const std::string &data):
            _size(data.size()),
            _buffer(data)
        {}
        [[nodiscard]] uint8_t *GetMemory() {
            return (uint8_t *) _buffer.data();
        }
        [[nodiscard]] std::size_t GetSize() {
            return _size;
        }
        const std::size_t _size;
        std::string _buffer;
    };
    typedef std::shared_ptr<StringSizedBuffer> StringSizedBufferPtr;

    struct VectorSizedBuffer: public SizedBuffer {
        explicit VectorSizedBuffer(const std::size_t size):
            _size(size),
            _buffer(size)
        {}
        [[nodiscard]] uint8_t *GetMemory() {
            return _buffer.data();
        }
        [[nodiscard]] std::size_t GetSize() {
            return _size;
        }
        const std::size_t _size;
        std::vector<uint8_t> _buffer;
    };

}

#endif //UTILS_BUFFERS_HPP
