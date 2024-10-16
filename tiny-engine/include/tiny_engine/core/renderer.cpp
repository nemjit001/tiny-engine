#include "tiny_engine/core/renderer.hpp"

#define VOLK_IMPLEMENTATION
#include <volk.h>

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
	}

	Renderer::~Renderer()
	{
		volkFinalize();
	}
} // namespace tiny_engine::core
