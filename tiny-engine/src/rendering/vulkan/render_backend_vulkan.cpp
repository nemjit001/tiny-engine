#include "rendering/vulkan/render_backend_vulkan.hpp"

#include <array>
#include <cstring>

#define VOLK_IMPLEMENTATION
#include <volk.h>

#include "engine_config.h"
#include "tiny_engine/application.hpp"
#include "tiny_engine/error_handling.hpp"
#include "tiny_engine/core/logging.hpp"
#include "tiny_engine/core/window_system.hpp"

namespace tiny_engine::rendering
{
	/// @brief The VulkanLoader class handles loading and unloading Vulkan at runtime.
	class VulkanLoader
	{
	public:
		VulkanLoader();
		~VulkanLoader();

		void loadInstance(VkInstance instance);
	};

	namespace
	{
#if		TINY_ENGINE_BUILD_DEBUG
		std::array s_ValidationLayers = {
			"VK_LAYER_KHRONOS_validation",
			"VK_LAYER_KHRONOS_synchronization2",
		};

		std::array s_InstanceDebugExtensions = {
			VK_EXT_DEBUG_UTILS_EXTENSION_NAME,
		};
#endif

		VulkanLoader s_VulkanLoader;

		bool vkBadResult(VkResult result)
		{
			return result != VK_SUCCESS;
		}
	} // namespace

	VulkanLoader::VulkanLoader()
	{
		if (vkBadResult(volkInitialize())) {
			TINY_ENGINE_DIE("Failed to load Vulkan");
		}

		uint32_t const vkVersion = volkGetInstanceVersion();
		core::Logging::Log(core::LogLevel::Info, "Loaded Vulkan (v%u.%u.%u)\n",
			VK_API_VERSION_MAJOR(vkVersion), VK_API_VERSION_MINOR(vkVersion), VK_API_VERSION_PATCH(vkVersion)
		);
	}

	VulkanLoader::~VulkanLoader()
	{
		volkFinalize();
	}

	void VulkanLoader::loadInstance(VkInstance instace)
	{
		volkLoadInstance(instace);
	}

	VulkanRenderBackend::VulkanRenderBackend(IApplication* pApplication, core::WindowSystem* pWindowSystem)
	{
		uint32_t windowExtCount = 0;
		char const** ppWindowExtensions = pWindowSystem->requiredVulkanInstanceExtensions(&windowExtCount);

		std::vector<char const*> enabledLayers;
		std::vector<char const*> enabledExtensions(ppWindowExtensions, ppWindowExtensions + windowExtCount);

#if		TINY_ENGINE_BUILD_DEBUG
		enabledLayers.insert(enabledLayers.end(), s_ValidationLayers.begin(), s_ValidationLayers.end());
		enabledExtensions.insert(enabledExtensions.begin(), s_InstanceDebugExtensions.begin(), s_InstanceDebugExtensions.end());
#endif

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
		dbgMessengerCreateInfo.pfnUserCallback = VulkanRenderBackend::debugCallback;
		dbgMessengerCreateInfo.pUserData = nullptr;

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
#if		TINY_ENGINE_BUILD_DEBUG
		instanceCreateInfo.pNext = &dbgMessengerCreateInfo;
#endif

		if (vkBadResult(vkCreateInstance(&instanceCreateInfo, nullptr, &instance))) {
			TINY_ENGINE_DIE("Failed to create Vulkan instance");
		}

		s_VulkanLoader.loadInstance(instance);

		if (vkCreateDebugUtilsMessengerEXT != nullptr && vkBadResult(vkCreateDebugUtilsMessengerEXT(instance, &dbgMessengerCreateInfo, nullptr, &dbgMessenger))) {
			TINY_ENGINE_DIE("Failed to create Vulkan debug messenger");
		}

		uint32_t deviceCount = 0;
		vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
		m_devices.resize(deviceCount);
		m_deviceInfos.resize(deviceCount);
		vkEnumeratePhysicalDevices(instance, &deviceCount, m_devices.data());

		for (uint32_t i = 0; i < deviceCount; i++)
		{
			VkPhysicalDevice const& device = m_devices[i];
			GraphicsDevice& deviceInfo = m_deviceInfos[i];

			VkPhysicalDeviceProperties properties{};
			vkGetPhysicalDeviceProperties(device, &properties);
			
			// FIXME(nemjit001): change this to safe in-house strcpy impl
			strncpy_s(deviceInfo.name, properties.deviceName, GraphicsDeviceNameLength);
			deviceInfo.deviceID = properties.deviceID;
		}
	}

	VulkanRenderBackend::~VulkanRenderBackend()
	{
		if (vkDestroyDebugUtilsMessengerEXT != nullptr) {
			vkDestroyDebugUtilsMessengerEXT(instance, dbgMessenger, nullptr);
		}

		vkDestroyInstance(instance, nullptr);
	}

	GraphicsDevice const* TINY_ENGINE_APICALL VulkanRenderBackend::enumerateDevices(uint32_t& count) const
	{
		count = static_cast<uint32_t>(m_deviceInfos.size());
		return m_deviceInfos.data();
	}

	VKAPI_ATTR VkBool32 VKAPI_CALL VulkanRenderBackend::debugCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT severity,
		VkDebugUtilsMessageTypeFlagsEXT type,
		VkDebugUtilsMessengerCallbackDataEXT const* pCallbackData,
		void* pUserData
	)
	{
		TINY_ENGINE_MARK_UNUSED(type);
		TINY_ENGINE_MARK_UNUSED(pUserData);

		switch (severity)
		{
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
			core::Logging::Log(core::LogLevel::Trace, "[Vulkan] %s\n", pCallbackData->pMessage);
			break;
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
			core::Logging::Log(core::LogLevel::Info, "[Vulkan] %s\n", pCallbackData->pMessage);
			break;
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
			core::Logging::Log(core::LogLevel::Warning, "[Vulkan] %s\n", pCallbackData->pMessage);
			break;
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
			core::Logging::Log(core::LogLevel::Error, "[Vulkan] %s\n", pCallbackData->pMessage);
			break;
		default:
			break;
		}

		return VK_FALSE;
	}
} // namespace tiny_engine::rendering
