#pragma once

#include <cstdint>

#include "tiny_engine/defines.hpp"

namespace tiny_engine::rendering
{
    static constexpr uint32_t GraphicsDeviceNameLength = 256;

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
    };
} // namespace tiny_engine::rendering
