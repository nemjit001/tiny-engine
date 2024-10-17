#include "tiny_engine/core/renderer.hpp"

#include <array>
#include <cstdio>
#include <vector>

#define VOLK_IMPLEMENTATION
#include <volk.h>

#include "engine_config.h"
#include "tiny_engine/defines.hpp"
#include "tiny_engine/error_handling.hpp"

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

		TINY_ENGINE_UNUSED std::array s_DeviceExtensions = {
			VK_KHR_SWAPCHAIN_EXTENSION_NAME,
		};

		TINY_ENGINE_UNUSED VKAPI_CALL VkBool32 VKAPI_ATTR vulkanDebugCallback(
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

		std::vector<char const*> enabledLayers;
		std::vector<char const*> enabledExtensions;

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

		VkApplicationInfo appInfo{};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName = nullptr;
		appInfo.applicationVersion = 0;
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
			TINY_ENGINE_DIE("Failed to create Vulkan instance");
		}

		volkLoadInstance(m_instance);

#if 	TINY_ENGINE_BUILD_DEBUG
		if (vkCreateDebugUtilsMessengerEXT == nullptr
			|| vkCreateDebugUtilsMessengerEXT(m_instance, &dbgMessengerCreateInfo, nullptr, &m_dbgMessenger) != VK_SUCCESS) {
			TINY_ENGINE_DIE("Failed to create Vulkan debug messenger");
		}
#elif	TINY_ENGINE_BUILD_RELEASE
		TINY_ENGINE_MARK_UNUSED(m_dbgMessenger);
#endif
	}

	Renderer::~Renderer()
	{
#if 	TINY_ENGINE_BUILD_DEBUG
		if (vkDestroyDebugUtilsMessengerEXT != nullptr) {
			vkDestroyDebugUtilsMessengerEXT(m_instance, m_dbgMessenger, nullptr);
		}
#endif

		vkDestroyInstance(m_instance, nullptr);
		volkFinalize();
	}
} // namespace tiny_engine::core
