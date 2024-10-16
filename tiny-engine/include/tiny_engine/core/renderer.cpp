#include "tiny_engine/core/renderer.hpp"

#include <cstdio>

#define VOLK_IMPLEMENTATION
#include <volk.h>

#include "engine_config.h"
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
		instanceCreateInfo.enabledLayerCount = 0;
		instanceCreateInfo.ppEnabledLayerNames = nullptr;
		instanceCreateInfo.enabledExtensionCount = 0;
		instanceCreateInfo.ppEnabledExtensionNames = nullptr;
		instanceCreateInfo.pNext = nullptr;

		if (vkFailed(vkCreateInstance(&instanceCreateInfo, nullptr, &m_instance))) {
			TINY_ENGINE_DIE("Failed to create Vulkan instance");
		}

		volkLoadInstance(m_instance);
	}

	Renderer::~Renderer()
	{
		vkDestroyInstance(m_instance, nullptr);
		volkFinalize();
	}
} // namespace tiny_engine::core
