#pragma once

#include <volk.h>

namespace tiny_engine::core
{
	class Renderer
	{
	public:
		Renderer();
		~Renderer();

		Renderer(Renderer const&) = delete;
		Renderer& operator=(Renderer const&) = delete;

	private:
		VkInstance m_instance = VK_NULL_HANDLE;
		VkDebugUtilsMessengerEXT m_dbgMessenger = VK_NULL_HANDLE;
	};
} // namespace tiny_engine::core
