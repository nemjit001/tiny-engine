#pragma once

#include <cstdint>
#include <memory>

#include "tiny_engine/defines.hpp"

namespace tiny_engine::rendering
{
    static constexpr uint32_t GraphicsDeviceNameLength = 256;

    class IRenderDevice;

    struct GraphicsDevice
    {
        char name[GraphicsDeviceNameLength];
        uint32_t deviceID;
    };

    class TINY_ENGINE_API IRenderBackend
    {
    public:
        IRenderBackend() = default;
        virtual ~IRenderBackend() = default;

        IRenderBackend(IRenderBackend const&) = delete;
        IRenderBackend& operator=(IRenderBackend const&) = delete;

        virtual GraphicsDevice const* TINY_ENGINE_APICALL enumerateDevices(uint32_t& count) const = 0;

        virtual std::unique_ptr<IRenderDevice> createRenderDevice(GraphicsDevice const& device) = 0;

        virtual char const* name() const = 0;
    };

    class TINY_ENGINE_API IRenderDevice
    {
    public:
        IRenderDevice() = default;
        virtual ~IRenderDevice() = default;

        IRenderDevice(IRenderDevice const&) = delete;
        IRenderDevice& operator=(IRenderDevice const&) = delete;

        // TODO(nemjit001):
        // - Resource management (texture, buffers, etc. Make use of handle based approach to avoid API specific types)
        //   - Resource view support (Must be generated from resources using device, also use handle based approach)
        // - CommandList management (Mainly allocation, synchronization)
        // - Shader args management (Determines shader argument layout, may allocate resources and must allow for patial rebind)
        // - Shader pipeline management (Allow compute & raster pipelines, base layout on shader args)
        // - Render Pass management (record render pass configuration with bound targets for framebuffer)
    };
} // namespace tiny_engine::rendering
