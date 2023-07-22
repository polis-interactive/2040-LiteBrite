//
// Created by broosegoose on 7/22/23.
//

#include "graphics.hpp"

namespace infrastructure {

    GraphicsPtr Graphics::Create(const GraphicsConfig &config, GraphicsManagerPtr manager) {
        auto graphics = std::make_shared<Graphics>(config, std::move(manager));
        return std::move(graphics);
    }

    Graphics::Graphics(const GraphicsConfig &config, GraphicsManagerPtr manager):
        _manager(std::move(manager))
    {

        const auto &layout = config.installation_layout;
        const auto &conf = config.installation_config;
        const auto &display = config.display;

        generateBuffers(layout.universes, display.buffer_count);

        if (conf.rgbw_pixels) {
            assert(conf.white_color.has_value() || conf.color_temperature.has_value());
            if (conf.white_color.has_value()) {
                _white_color = conf.white_color.value();
            } else {
                _white_color = domain::CRGB::FromColorTemperature(conf.color_temperature.value());
            }
        }

        _frame_time = utils::QuickDuration(1.0 / display.fps);

        _display_type = display.type;
        if (display.rgb_color.has_value()) {
            _rgb_color = display.rgb_color.value();
        }
        if (display.rgbw_color.has_value()) {
            _rgbw_color = display.rgbw_color.value();
        }
    }

    void Graphics::generateBuffers(const domain::UniverseMap &universes, const unsigned int buffer_count) {
        auto max_universe = std::max_element(universes.begin(), universes.end(),
            [](const auto& a, const auto& b) {
                return a.second.GetLastLength() > b.second.GetLastLength();
            }
        );
        const auto buffer_size = max_universe->second.GetLastLength();
        for (int i = 0; i < buffer_count; i++) {
            _buffers.push(new GraphicsBuffer(buffer_size));
        }
    }

    Graphics::~Graphics() {
        Stop();
    }

    void Graphics::Start() {
        if (_started) {
            return;
        }
        _started = true;
        _graphics_thread = std::make_unique<std::jthread>(std::bind_front(&Graphics::run, shared_from_this()));
    }

    void Graphics::Stop() {
        if (!_started) {
            return;
        }
        _started = false;
        if (_graphics_thread != nullptr) {
            _graphics_thread->request_stop();
            _graphics_thread->join();
        }
        _graphics_thread = nullptr;
    }

    void Graphics::run(const std::stop_token &st) {
        while(!st.stop_requested()) {
            const auto start = utils::Clock::now();
            GraphicsBufferPtr buffer = getBuffer();
            if (buffer != nullptr) {
                render(buffer);
                _manager->PostGraphicsUpdate(std::move(buffer));
            }
            const auto end = utils::Clock::now();
            utils::QuickDuration elapsed_time = end - start;
            if (elapsed_time < _frame_time) {
                std::this_thread::sleep_for(_frame_time - elapsed_time);
            }
            /*
             * ideally we can ensure we hit a good fps here; but it probably doesn't matter with
             * our current dumb renderer
             */
        }
    }

    GraphicsBufferPtr Graphics::getBuffer() {
        GraphicsBuffer *buffer = nullptr;
        {
            std::lock_guard<std::mutex> lk(_buffer_lock);
            if (!_buffers.empty()) {
                buffer = _buffers.front();
                _buffers.pop();
            }
        }
        if (buffer == nullptr) {
            return nullptr;
        }
        auto buffer_ptr = std::shared_ptr<GraphicsBuffer>(
            buffer, std::bind_front(&Graphics::queueBuffer, shared_from_this())
        );
        return std::move(buffer_ptr);
    }

    void Graphics::render(GraphicsBufferPtr &buffer) {
        /* i still need color correction, maybe gamma, but great for now */
        switch (_display_type) {
            case domain::DisplayType::RGBW:
                buffer->RenderColor(_rgbw_color);
                break;
            case domain::DisplayType::RGB:
                buffer->RenderColor(_rgb_color);
                break;
            case domain::DisplayType::RGB_WITH_W_INTERPOLATION:
                auto c_rgbw = domain::CRGB::SubtractWhite(_rgb_color, _white_color);
                buffer->RenderColor(c_rgbw);
                break;
        }
    }

    void Graphics::queueBuffer(GraphicsBuffer *buffer) {
        std::lock_guard<std::mutex> lk(_buffer_lock);
        _buffers.push(buffer);
    }

    void GraphicsBuffer::RenderColor(const domain::CRGB &c_rgb) {
        // Fill the first 3 bytes.
        std::memcpy((void *) _data.data(), c_rgb.raw, 3);

        // Now copy in doubling sizes.
        size_t bytes_filled = 3;
        while (bytes_filled + 3 < _size) {
            size_t bytes_to_copy = std::min(bytes_filled, _size - bytes_filled);
            std::memcpy((void *) (_data.data() + bytes_filled), _data.data(), bytes_to_copy);
            bytes_filled += bytes_to_copy;
        }
    }
    void GraphicsBuffer::RenderColor(const domain::CRGBW &c_rgbw) {
        // Fill the first 4 bytes.
        std::memcpy((void *) _data.data(), c_rgbw.raw, 3);

        // Now copy in doubling sizes.
        size_t bytes_filled = 4;
        while (bytes_filled + 4 < _size) {
            size_t bytes_to_copy = std::min(bytes_filled, _size - bytes_filled);
            std::memcpy((void *) (_data.data() + bytes_filled), _data.data(), bytes_to_copy);
            bytes_filled += bytes_to_copy;
        }
    }
}