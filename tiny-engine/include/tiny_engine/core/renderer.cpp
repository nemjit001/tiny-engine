#include "tiny_engine/core/renderer.hpp"

#include <array>
#include <cstdio>
#include <vector>

#define VOLK_IMPLEMENTATION
#include <volk.h>

#include "engine_config.h"
#include "tiny_engine/application.hpp"
#include "tiny_engine/defines.hpp"
#include "tiny_engine/error_handling.hpp"
#include "tiny_engine/core/window_system.hpp"

namespace tiny_engine::core
{
	static constexpr bool vkSucceeded(VkResult result)
	{
		return result == VK_SUCCESS;
	}

	static constexpr bool vkFailed(VkResult result)
	{
		return !vkSucceeded(result);
	}

	namespace
	{
#if 	TINY_ENGINE_BUILD_DEBUG
		std::array s_ValidationLayers = {
			"VK_LAYER_KHRONOS_validation",
			"VK_LAYER_KHRONOS_synchronization2",
		};

		std::array s_InstaceDebugExtensions = {
			VK_EXT_DEBUG_UTILS_EXTENSION_NAME,
		};
#endif

		std::array s_DeviceExtensions = {
			VK_KHR_SWAPCHAIN_EXTENSION_NAME,
		};

		TINY_ENGINE_UNUSED VKAPI_ATTR VkBool32 VKAPI_CALL vulkanDebugCallback(
			VkDebugUtilsMessageSeverityFlagBitsEXT severity,
			VkDebugUtilsMessageTypeFlagsEXT type,
			VkDebugUtilsMessengerCallbackDataEXT const* pCallbackData,
			void* pUserData
		)
		{
			TINY_ENGINE_MARK_UNUSED(severity);
			TINY_ENGINE_MARK_UNUSED(type);
			TINY_ENGINE_MARK_UNUSED(pUserData);

			printf("[Vulkan] %s\n", pCallbackData->pMessage);
			return VK_FALSE;
		}
	} // namespace

	struct SwapchainCapabilities
	{
		VkSurfaceCapabilitiesKHR surfaceCapabilities;
		std::vector<VkSurfaceFormatKHR> surfaceFormats;
		std::vector<VkPresentModeKHR> presentModes;

		static SwapchainCapabilities getSwapchainCapabilities(VkPhysicalDevice device, VkSurfaceKHR surface)
		{
			SwapchainCapabilities caps{};
			vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &caps.surfaceCapabilities);

			uint32_t formatCount = 0;
			vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);
			caps.surfaceFormats.resize(formatCount);
			vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, caps.surfaceFormats.data());

			uint32_t modeCount = 0;
			vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &modeCount, nullptr);
			caps.presentModes.resize(modeCount);
			vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &modeCount, caps.presentModes.data());

			return caps;
		}

		uint32_t getImageCount(uint32_t preferredCount) const
		{
			uint32_t count = surfaceCapabilities.minImageCount + 1;
			if (count < preferredCount) {
				count = preferredCount;
			}

			if (count > surfaceCapabilities.maxImageCount && surfaceCapabilities.maxImageCount != 0) {
				count = surfaceCapabilities.maxImageCount;
			}

			return count;
		}

		VkExtent2D getImageExtent(VkExtent2D const& preferredExtent) const
		{
			VkExtent2D extent = surfaceCapabilities.currentExtent;
			if (extent.width == UINT32_MAX || extent.height == UINT32_MAX) {
				extent = preferredExtent;
			}

			return extent;
		}

		VkPresentModeKHR getPresentMode(VSyncMode vsyncMode) const
		{
			VkPresentModeKHR preferredMode = VK_PRESENT_MODE_FIFO_KHR;
			switch (vsyncMode)
			{
			case VSyncMode::Enabled:
				preferredMode = VK_PRESENT_MODE_FIFO_KHR;
				break;
			case VSyncMode::Disabled:
				preferredMode = VK_PRESENT_MODE_MAILBOX_KHR;
				break;
			case VSyncMode::DisabledTearing:
				preferredMode = VK_PRESENT_MODE_IMMEDIATE_KHR;
				break;
			default:
				break;
			}

			for (auto const& available : presentModes)
			{
				if (available == preferredMode) {
					return preferredMode;
				}
			}

			return VK_PRESENT_MODE_FIFO_KHR; //> guaranteed to be supported by the Vulkan spec
		}
	};

	Renderer::Renderer()
	{
		if (vkFailed(volkInitialize())) {
			TINY_ENGINE_DIE("Vulkan not available");
		}

		uint32_t const version = volkGetInstanceVersion();
		printf("Loaded Vulkan (v%u.%u.%u)\n",
			VK_API_VERSION_MAJOR(version),
			VK_API_VERSION_MINOR(version),
			VK_API_VERSION_PATCH(version)
		);
	}

	Renderer::~Renderer()
	{
		volkFinalize();
	}

	RendererInitResult Renderer::init(WindowSystem* pWindowSystem, IApplication const* pApplication, VSyncMode vsyncMode)
	{
		uint32_t windowExtCount = 0;
		char const** ppWindowExtensions = pWindowSystem->requiredVulkanInstanceExtensions(&windowExtCount);

		std::vector<char const*> enabledLayers;
		std::vector<char const*> enabledExtensions(ppWindowExtensions, ppWindowExtensions + windowExtCount);

#if 	TINY_ENGINE_BUILD_DEBUG
		enabledLayers.insert(enabledLayers.end(), s_ValidationLayers.begin(), s_ValidationLayers.end());
		enabledExtensions.insert(enabledExtensions.end(), s_InstaceDebugExtensions.begin(), s_InstaceDebugExtensions.end());

		VkDebugUtilsMessengerCreateInfoEXT dbgMessengerCreateInfo{};
		dbgMessengerCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
		dbgMessengerCreateInfo.flags = 0;
		dbgMessengerCreateInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT
			| VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT
			| VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT
			| VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
		dbgMessengerCreateInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT
			| VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT
			| VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT
			| VK_DEBUG_UTILS_MESSAGE_TYPE_DEVICE_ADDRESS_BINDING_BIT_EXT;
		dbgMessengerCreateInfo.pfnUserCallback = vulkanDebugCallback;
		dbgMessengerCreateInfo.pUserData = nullptr;
#endif

		AppVersion const appVersion = pApplication->version();
		VkApplicationInfo appInfo{};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName = pApplication->name();
		appInfo.applicationVersion = VK_MAKE_API_VERSION(0, appVersion.major, appVersion.minor, appVersion.patch);
		appInfo.pEngineName = TINY_ENGINE_NAME;
		appInfo.engineVersion = VK_MAKE_API_VERSION(0, TINY_ENGINE_VERSION_MAJOR, TINY_ENGINE_VERSION_MINOR, TINY_ENGINE_VERSION_PATCH);
		appInfo.apiVersion = VK_API_VERSION_1_3;

		VkInstanceCreateInfo instanceCreateInfo{};
		instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		instanceCreateInfo.flags = 0;
		instanceCreateInfo.pApplicationInfo = &appInfo;
		instanceCreateInfo.enabledLayerCount = static_cast<uint32_t>(enabledLayers.size());
		instanceCreateInfo.ppEnabledLayerNames = enabledLayers.data();
		instanceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(enabledExtensions.size());
		instanceCreateInfo.ppEnabledExtensionNames = enabledExtensions.data();
		instanceCreateInfo.pNext = nullptr;
#if 	TINY_ENGINE_BUILD_DEBUG
		instanceCreateInfo.pNext = &dbgMessengerCreateInfo;
#endif

		if (vkFailed(vkCreateInstance(&instanceCreateInfo, nullptr, &m_instance))) {
			printf("Failed to create Vulkan instance\n");
			return RendererInitResult::BasicInitFailed;
		}

		volkLoadInstance(m_instance);

#if 	TINY_ENGINE_BUILD_DEBUG
		if (vkCreateDebugUtilsMessengerEXT == nullptr
			|| vkFailed(vkCreateDebugUtilsMessengerEXT(m_instance, &dbgMessengerCreateInfo, nullptr, &m_dbgMessenger))) {
			printf("Failed to create Vulkan debug messenger\n");
			return RendererInitResult::BasicInitFailed;
		}
#elif	TINY_ENGINE_BUILD_RELEASE
		TINY_ENGINE_MARK_UNUSED(m_dbgMessenger);
#endif

		if (vkFailed(pWindowSystem->createVulkanWindowSurface(m_instance, nullptr, &m_surface))) {
			printf("Failed to create Vulkan surface\n");
			return RendererInitResult::BadSurface;
		}

		uint32_t deviceCount = 0;
		vkEnumeratePhysicalDevices(m_instance, &deviceCount, nullptr);
		std::vector<VkPhysicalDevice> devices(deviceCount);
		vkEnumeratePhysicalDevices(m_instance, &deviceCount, devices.data());
		for (auto const& device : devices)
		{
			VkPhysicalDeviceProperties deviceProperties{};
			vkGetPhysicalDeviceProperties(device, &deviceProperties);
			printf("Found render device: (0x%X) [%s]\n", deviceProperties.deviceID, deviceProperties.deviceName);
		}

		m_GPU = devices[0];

		uint32_t queueFamilyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(m_GPU, &queueFamilyCount, nullptr);
		std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(m_GPU, &queueFamilyCount, queueFamilies.data());

		VkQueueFlags const generalPurposeQueueFlags = VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_COMPUTE_BIT | VK_QUEUE_TRANSFER_BIT;
		VkBool32 const generalPurposeRequireSurfaceSupport = VK_TRUE;
		m_generalPurposeQueueFamily = VK_QUEUE_FAMILY_IGNORED;

		for (size_t queueIdx = 0; queueIdx < queueFamilies.size(); queueIdx++)
		{
			VkBool32 surfaceSupport = VK_FALSE;
			vkGetPhysicalDeviceSurfaceSupportKHR(m_GPU, static_cast<uint32_t>(queueIdx), m_surface, &surfaceSupport);

			VkQueueFamilyProperties const& queueProperties = queueFamilies[queueIdx];
			if ((queueProperties.queueFlags & generalPurposeQueueFlags) == generalPurposeQueueFlags)
			{
				if (generalPurposeRequireSurfaceSupport && !surfaceSupport) {
					continue;
				}

				m_generalPurposeQueueFamily = static_cast<uint32_t>(queueIdx);
			}
		}

		float priorities[] = { 1.0F };
		VkDeviceQueueCreateInfo queueCreateInfo{};
		queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueCreateInfo.flags = 0;
		queueCreateInfo.queueFamilyIndex = m_generalPurposeQueueFamily;
		queueCreateInfo.queueCount = TINY_ENGINE_SIZEOF_ARRAY(priorities);
		queueCreateInfo.pQueuePriorities = priorities;

		VkDeviceCreateInfo deviceCreateInfo{};
		deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		deviceCreateInfo.flags = 0;
		deviceCreateInfo.queueCreateInfoCount = 1;
		deviceCreateInfo.pQueueCreateInfos = &queueCreateInfo;
		deviceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(s_DeviceExtensions.size());
		deviceCreateInfo.ppEnabledExtensionNames = s_DeviceExtensions.data();
		deviceCreateInfo.pEnabledFeatures = nullptr;
		deviceCreateInfo.pNext = nullptr;

		if (vkFailed(vkCreateDevice(m_GPU, &deviceCreateInfo, nullptr, &m_device))) {
			printf("Failed to create Vulkan device\n");
			return RendererInitResult::BadDevice;
		}
		vkGetDeviceQueue(m_device, m_generalPurposeQueueFamily, 0, &m_generalPurposeQueue);
		
		WindowSize const currentWindowSize = pWindowSystem->getWindowSize();
		recreateSwapchain(currentWindowSize, vsyncMode);

		for (auto& fence : m_swapImageAvailable)
		{
			VkFenceCreateInfo fenceCreateInfo{};
			fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
			fenceCreateInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

			if (vkFailed(vkCreateFence(m_device, &fenceCreateInfo, nullptr, &fence))) {
				return RendererInitResult::BadSyncInit;
			}
		}

		return RendererInitResult::OK;
	}

	void Renderer::shutdown()
	{
		for (auto& fence : m_swapImageAvailable) {
			vkDestroyFence(m_device, fence, nullptr);
		}

		vkDestroySwapchainKHR(m_device, m_swapchain, nullptr);
		vkDestroyDevice(m_device, nullptr);

		vkDestroySurfaceKHR(m_instance, m_surface, nullptr);
#if 	TINY_ENGINE_BUILD_DEBUG
		if (vkDestroyDebugUtilsMessengerEXT != nullptr) {
			vkDestroyDebugUtilsMessengerEXT(m_instance, m_dbgMessenger, nullptr);
		}
#endif
		vkDestroyInstance(m_instance, nullptr);
	}

	void Renderer::recreateSwapchain(WindowSize const& size, VSyncMode vsyncMode)
	{
		// TODO(nemjit001): Await swap resource usage here

		VkSwapchainKHR oldSwapchain = m_swapchain;
		SwapchainCapabilities const swapCapabilities = SwapchainCapabilities::getSwapchainCapabilities(m_GPU, m_surface);
		m_vsyncMode = vsyncMode;
		m_swapExtent = swapCapabilities.getImageExtent(VkExtent2D{ size.width, size.height });

		// TODO(nemjit001): Reuse as much as possible from swap creation
		VkSwapchainCreateInfoKHR swapchainCreateInfo{};
		swapchainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		swapchainCreateInfo.flags = 0;
		swapchainCreateInfo.surface = m_surface;
		swapchainCreateInfo.minImageCount = swapCapabilities.getImageCount(SwapImageCount);
		swapchainCreateInfo.imageFormat = SwapImageFormat;
		swapchainCreateInfo.imageColorSpace = VK_COLORSPACE_SRGB_NONLINEAR_KHR;
		swapchainCreateInfo.imageExtent = m_swapExtent;
		swapchainCreateInfo.imageArrayLayers = 1;
		swapchainCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
		swapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		swapchainCreateInfo.queueFamilyIndexCount = 0;
		swapchainCreateInfo.pQueueFamilyIndices = nullptr;
		swapchainCreateInfo.preTransform = swapCapabilities.surfaceCapabilities.currentTransform;
		swapchainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		swapchainCreateInfo.presentMode = swapCapabilities.getPresentMode(m_vsyncMode);
		swapchainCreateInfo.clipped = VK_FALSE;
		swapchainCreateInfo.oldSwapchain = oldSwapchain;

		if (vkFailed(vkCreateSwapchainKHR(m_device, &swapchainCreateInfo, nullptr, &m_swapchain))) {
			TINY_ENGINE_DIE("Failed to recreate Vulkan swapchain\n");
		}

		if (oldSwapchain != VK_NULL_HANDLE) {
			vkDestroySwapchainKHR(m_device, oldSwapchain, nullptr);
		}

		uint32_t swapImageCount = 0;
		vkGetSwapchainImagesKHR(m_device, m_swapchain, &swapImageCount, nullptr);
		m_swapImages.resize(swapImageCount);
		vkGetSwapchainImagesKHR(m_device, m_swapchain, &swapImageCount, m_swapImages.data());
	}

	void Renderer::startFrame()
	{
		m_acquireFailed = false;
		if (vkFailed(vkResetFences(m_device, 1, &m_swapImageAvailable[m_frameIndex]))) {
			TINY_ENGINE_DIE("Failed to reset Swap chain fences\n");
		}

		VkResult result = vkAcquireNextImageKHR(m_device, m_swapchain, UINT64_MAX, VK_NULL_HANDLE, m_swapImageAvailable[m_frameIndex], &m_backbufferIndex);
		if (result != VK_SUCCESS && (result == VK_SUBOPTIMAL_KHR || result == VK_ERROR_OUT_OF_DATE_KHR)) {
			m_acquireFailed = true;
			return;
		}
		else if (result != VK_SUCCESS) {
			TINY_ENGINE_DIE("Failed to acquire next Swap chain image\n");
		}

		if (vkFailed(vkWaitForFences(m_device, 1, &m_swapImageAvailable[m_frameIndex], VK_TRUE, UINT64_MAX))) {
			TINY_ENGINE_DIE("Failed to wait on Swap chain fences\n");
		}
	}

	void Renderer::presentFrame()
	{
		if (m_acquireFailed) {
			return;
		}

		// TODO(nemjit001): Present image
		VkPresentInfoKHR presentInfo{};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		presentInfo.waitSemaphoreCount = 0;
		presentInfo.pWaitSemaphores = nullptr;
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = &m_swapchain;
		presentInfo.pImageIndices = &m_backbufferIndex;
		presentInfo.pResults = nullptr;

		VkResult result = vkQueuePresentKHR(m_generalPurposeQueue, &presentInfo);
		if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR && result != VK_ERROR_OUT_OF_DATE_KHR) {
			TINY_ENGINE_DIE("Failed to present Swap image\n");
		}

		m_frameIndex = (m_frameIndex + 1) % FramesInFlight;
	}

	WindowSize Renderer::framebufferSize() const
	{
		return WindowSize{ m_swapExtent.width, m_swapExtent.height };
	}

	VSyncMode Renderer::vsyncMode() const
	{
		return m_vsyncMode;
	}

	uint32_t Renderer::backbufferCount() const
	{
		return static_cast<uint32_t>(m_swapImages.size());
	}

	uint32_t Renderer::backbufferIndex() const
	{
		return m_backbufferIndex;
	}
} // namespace tiny_engine::core
