#pragma once

#include <vector>

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
		BadDevice,
		BadSwapchain,
		BadSyncInit,
	};

	enum class VSyncMode : uint8_t
	{
		Enabled = 0,
		Disabled,
		DisabledTearing,
	};

	class Renderer
	{
	public:
		Renderer();
		~Renderer();

		Renderer(Renderer const&) = delete;
		Renderer& operator=(Renderer const&) = delete;

		RendererInitResult init(WindowSystem* pWindowSystem, IApplication const* pApplication, VSyncMode vsyncMode);

		void shutdown();

		void recreateSwapchain(WindowSize const& size, VSyncMode vsyncMode);

		void startFrame();

		void presentFrame();

		WindowSize framebufferSize() const;

		VSyncMode vsyncMode() const;

		uint32_t backbufferCount() const;

		uint32_t backbufferIndex() const;

	private:
		static constexpr uint32_t FramesInFlight = 3;
		static constexpr uint32_t SwapImageCount = 3;
		static constexpr VkFormat SwapImageFormat = VK_FORMAT_B8G8R8A8_UNORM;

		VkInstance m_instance = VK_NULL_HANDLE;
		VkDebugUtilsMessengerEXT m_dbgMessenger = VK_NULL_HANDLE; //> unused in release builds
		VkSurfaceKHR m_surface = VK_NULL_HANDLE;

		VkPhysicalDevice m_GPU = VK_NULL_HANDLE;
		VkDevice m_device = VK_NULL_HANDLE;
		uint32_t m_generalPurposeQueueFamily = VK_QUEUE_FAMILY_IGNORED;
		VkQueue m_generalPurposeQueue = VK_NULL_HANDLE;

		VSyncMode m_vsyncMode = VSyncMode::Enabled;
		VkExtent2D m_swapExtent = VkExtent2D{};
		VkSwapchainKHR m_swapchain = VK_NULL_HANDLE;
		std::vector<VkImage> m_swapImages;

		uint32_t m_backbufferIndex = 0;
		uint32_t m_frameIndex = 0;
		bool m_acquireFailed = false;

		VkFence m_swapImageAvailable[FramesInFlight] = {};
	};
} // namespace tiny_engine::core
