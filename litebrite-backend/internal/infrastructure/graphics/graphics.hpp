//
// Created by broosegoose on 7/22/23.
//

#ifndef INFRASTRUCTURE_GRAPHICS_HPP
#define INFRASTRUCTURE_GRAPHICS_HPP

#include <thread>
#include <queue>

#include <nlohmann/json.hpp>

#include "utils/buffers.hpp"
#include "utils/clock.hpp"

#include "domain/color.hpp"
#include "domain/display.hpp"
#include "domain/installation.hpp"

namespace infrastructure {

    struct GraphicsConfig {
        domain::Display default_display;
        domain::installation::Config installation_config;
        domain::installation::Layout installation_layout;
        static GraphicsConfig from_json(const nlohmann::json& j) {
            GraphicsConfig conf{};
            conf.installation_config = domain::installation::Config::from_json(j.at("installation_config"));
            conf.installation_layout = domain::installation::Layout::from_json(j.at("installation_layout"));
            conf.default_display = domain::Display::from_json(j.at("default_display"));
            return conf;
        }
    };

    struct GraphicsManager {
        virtual void PostGraphicsUpdate(utils::SizedBufferPtr &&pixels) = 0;
    };
    typedef std::shared_ptr<GraphicsManager> GraphicsManagerPtr;

    class GraphicsBuffer: public utils::SizedBuffer {
    public:
        explicit GraphicsBuffer(const unsigned int buffer_size):
            _size(buffer_size),
            _data(std::vector<uint8_t>(buffer_size))
        {}
        void RenderColor(const domain::CRGB &c_rgb);
        void RenderColor(const domain::CRGBW &c_rgbw);
        [[nodiscard]] uint8_t *GetMemory() final {
            return _data.data();
        }
        [[nodiscard]] std::size_t GetSize() final {
            return _size;
        };
    private:
        const unsigned int _size;
        std::vector<uint8_t> _data;
    };
    typedef std::shared_ptr<GraphicsBuffer> GraphicsBufferPtr;

    class Graphics;
    typedef std::shared_ptr<Graphics> GraphicsPtr;

    class Graphics: public std::enable_shared_from_this<Graphics> {
    public:
        [[nodiscard]] static GraphicsPtr Create(const GraphicsConfig &config, GraphicsManagerPtr manager);
        explicit Graphics(const GraphicsConfig &config, GraphicsManagerPtr manager);
        void Start();
        void Stop();
        ~Graphics();
        // no copy assignment, no empty assignment
        Graphics() = delete;
        Graphics (const Graphics&) = delete;
        Graphics& operator= (const Graphics&) = delete;
    private:
        void generateBuffers(const domain::UniverseMap &universes, const unsigned int buffer_count);
        void run(const std::stop_token &st);
        [[nodiscard]] GraphicsBufferPtr getBuffer();
        void render(GraphicsBufferPtr &buffer);
        void queueBuffer(GraphicsBuffer *buffer);
        std::unique_ptr<std::jthread> _graphics_thread = nullptr;
        std::atomic_bool _started = false;

        GraphicsManagerPtr _manager;

        /* currently, forget about corrections; we need something running */
        domain::CRGB _white_color = {0};
        domain::CRGB _color_correction = {0};

        utils::QuickDuration _frame_time = 33ms;

        std::mutex _buffer_lock;
        std::queue<GraphicsBuffer *> _buffers;

        domain::DisplayType _display_type;
        domain::CRGB _rgb_color = { 0 };
        domain::CRGBW _rgbw_color = { 0 };

    };
}

#endif //INFRASTRUCTURE_GRAPHICS_HPP
