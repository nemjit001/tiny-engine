#pragma once

#include <volk.h>

namespace tiny_engine
{
	class IApplication;
} // namespace tiny_engine

namespace tiny_engine::core
{
	struct WindowSize;
	class WindowSystem;

	enum class RendererInitResult : uint8_t
	{
		OK = 0,
		BasicInitFailed,
		BadSurface,
	};

	class Renderer
	{
	public:
		Renderer();
		~Renderer();

		Renderer(Renderer const&) = delete;
		Renderer& operator=(Renderer const&) = delete;

		RendererInitResult init(WindowSystem* pWindowSystem, IApplication const* pApplication);

		void shutdown();

		void handleResize(WindowSize const& size);

	private:
		VkInstance m_instance = VK_NULL_HANDLE;
		VkDebugUtilsMessengerEXT m_dbgMessenger = VK_NULL_HANDLE; //> unused in release builds
		VkSurfaceKHR m_surface = VK_NULL_HANDLE;
	};
} // namespace tiny_engine::core
