#pragma once

#include <vector>
#include <volk.h>

#include "tiny_engine/rendering/render_backend.hpp"

namespace tiny_engine
{
	class IApplication;

	namespace core
	{
		class WindowSystem;
	} // namespace core
} // namespace tiny_engine

namespace tiny_engine::rendering
{
	class VulkanRenderBackend
		:
		public IRenderBackend
	{
	public:
		VulkanRenderBackend(IApplication* pApplication, core::WindowSystem* pWindowSystem);
		virtual ~VulkanRenderBackend();

		virtual GraphicsDevice const* TINY_ENGINE_APICALL enumerateDevices(uint32_t& count) const override final;

		static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
			VkDebugUtilsMessageSeverityFlagBitsEXT severity,
			VkDebugUtilsMessageTypeFlagsEXT type,
			VkDebugUtilsMessengerCallbackDataEXT const* pCallbackData,
			void* pUserData
		);

	public:
		VkInstance instance = VK_NULL_HANDLE;
		VkDebugUtilsMessengerEXT dbgMessenger = VK_NULL_HANDLE;

	private:
		std::vector<VkPhysicalDevice> m_devices;
		std::vector<GraphicsDevice> m_deviceInfos;
	};
} // namespace tiny_engine::rendering
