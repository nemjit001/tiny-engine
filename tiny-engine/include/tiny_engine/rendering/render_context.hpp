#pragma once

#include <cstdint>

#include "tiny_engine/defines.hpp"

namespace tiny_engine
{
    class IApplication;
} // namespace tiny_engine

namespace tiny_engine::core
{
    class WindowSystem;
} // namespace tiny_engine::core


namespace tiny_engine::rendering
{
    constexpr uint32_t GraphicsDeviceNameLength = 256;

    class RenderDeviceContext;
    class FrameManager;

    /// @brief The GraphicsDevice structure simply wraps a name & ID for a graphics device.
    struct GraphicsDevice
    {
        char name[GraphicsDeviceNameLength];
        uint32_t id;
    };

    /// @brief The RenderContext handles loading a backend graphics API and graphics device enumeration.
    class TINY_ENGINE_API RenderContext
    {
    public:
        RenderContext(core::WindowSystem* pWindowSystem, IApplication* pApplication);
        ~RenderContext();

        RenderContext(RenderContext const&) = delete;
        RenderContext& operator=(RenderContext const&) = delete;

        GraphicsDevice* TINY_ENGINE_APICALL enumerateDevices(uint32_t& count);

        RenderDeviceContext TINY_ENGINE_APICALL createDeviceContext(GraphicsDevice const& device);
    };

    /// @brief The RenderDeviceContext is used to create graphics API types and upload work to the GPU.
    class TINY_ENGINE_API RenderDeviceContext
    {
    public:
        RenderDeviceContext();
        ~RenderDeviceContext();

        RenderDeviceContext(RenderDeviceContext const&) = delete;
        RenderDeviceContext& operator=(RenderDeviceContext const&) = delete;

        FrameManager TINY_ENGINE_APICALL createFrameManager(core::WindowSystem* pWindowSystem);
    };

    /// @brief The FrameManager gives the renderer the notion of a "frame" and manages synchronization between frames.
    class TINY_ENGINE_API FrameManager
    {
    public:
        static constexpr uint32_t FramesInFlight = 3;

        FrameManager();
        ~FrameManager();

        FrameManager(FrameManager const&) = delete;
        FrameManager& operator=(FrameManager const&) = delete;
    };
} // namespace tiny_engine::rendering

