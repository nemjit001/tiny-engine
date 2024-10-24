#pragma once

#include <cstdint>
#include <memory>

#include "tiny_engine/defines.hpp"

namespace tiny_engine::rendering
{
    static constexpr uint32_t GraphicsDeviceNameLength = 256;

    class IRenderDevice;

    enum class CommandQueueType : int8_t
    {
        Graphics = 0x01,
        Transfer = 0x02,
        Compute = 0x04,
        All = Graphics | Transfer | Compute,
    };

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

        virtual std::unique_ptr<IRenderDevice> TINY_ENGINE_APICALL createRenderDevice(GraphicsDevice const& device) = 0;

        virtual char const* TINY_ENGINE_APICALL name() const = 0;
    };

    class CommandList;
    class Resource;
    class ShaderArgs;
    class ShaderModule;
    class ShaderPipeline;

    class TINY_ENGINE_API IRenderDevice
    {
    public:
        IRenderDevice() = default;
        virtual ~IRenderDevice() = default;

        IRenderDevice(IRenderDevice const&) = delete;
        IRenderDevice& operator=(IRenderDevice const&) = delete;

        virtual void TINY_ENGINE_APICALL submit(CommandQueueType queue, uint32_t count, CommandList* pCommandLists) = 0;

        virtual CommandList TINY_ENGINE_APICALL createCommandList(CommandQueueType queue) = 0;

        virtual void TINY_ENGINE_APICALL destroyCommandList(CommandList const& commandList) = 0;

        virtual Resource TINY_ENGINE_APICALL createResource() = 0;

        virtual void TINY_ENGINE_APICALL destroyResource(Resource const& resource) = 0;

        virtual ShaderArgs TINY_ENGINE_APICALL createShaderArgs() = 0;

        virtual void TINY_ENGINE_APICALL destroyShaderArgs(ShaderArgs const& args) = 0;

        virtual ShaderModule TINY_ENGINE_APICALL createShaderModule() = 0;

        virtual void TINY_ENGINE_APICALL destroyShaderModule(ShaderModule const& module) = 0;

        virtual ShaderPipeline TINY_ENGINE_APICALL createShaderPipeline() = 0;

        virtual void TINY_ENGINE_APICALL destroyShaderPipeline(ShaderPipeline const& pipeline) = 0;

        // TODO(nemjit001):
        // - Resource view support (Must be generated from resources using device)
        // - Render Pass management (record render pass configuration, allow bound targets for framebuffer)
    };

    class TINY_ENGINE_API CommandList
    {
    public:
        //
    };

    class TINY_ENGINE_API Resource
    {
    public:
        //
    };

    class TINY_ENGINE_API ShaderArgs
    {
    public:
        //
    };

    class TINY_ENGINE_API ShaderModule
    {
    public:
        //
    };

    class TINY_ENGINE_API ShaderPipeline
    {
    public:
        //
    };
} // namespace tiny_engine::rendering
