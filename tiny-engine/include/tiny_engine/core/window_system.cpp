#include "tiny_engine/core/window_system.hpp"

#if TINY_ENGINE_PLATFORM_WINDOWS
	#define GLFW_EXPOSE_NATIVE_WIN32
#endif

#if		TINY_ENGINE_VULKAN_AVAILABLE
	#define GLFW_INCLUDE_VULKAN
#endif

#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#include "tiny_engine/error_handling.hpp"

namespace tiny_engine::core
{
	namespace
	{
		bool s_WindowMinimized = false;
		GLFWwindow* s_pWindow = nullptr;
		WindowSystem::ResizeHandler s_ResizeHandler;
		WindowSystem::CloseHandler s_CloseHandler;

		void windowSizeCallback(GLFWwindow* pWindow, int width, int height)
		{
			if (pWindow != s_pWindow) {
				return;
			}

			s_WindowMinimized = (width == 0 || height == 0);
			if (!s_WindowMinimized && s_ResizeHandler) {
				s_ResizeHandler(WindowSize{ static_cast<uint32_t>(width), static_cast<uint32_t>(height) });
			}
		}

		void windowCloseCallback(GLFWwindow* pWindow)
		{
			if (pWindow != s_pWindow) {
				return;
			}

			if (s_CloseHandler) {
				s_CloseHandler();
			}
		}
	} // namespace

	WindowSystem::WindowSystem()
	{
		if (glfwInit() == GLFW_FALSE) {
			TINY_ENGINE_DIE("Failed to initialize GLFW");
		}
	}

	WindowSystem::~WindowSystem()
	{
		glfwTerminate();
	}

	bool TINY_ENGINE_APICALL WindowSystem::init(WindowDesc const& window)
	{
		if (s_pWindow != nullptr) {
			return false;
		}

		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, window.resizable);
		s_pWindow = glfwCreateWindow(static_cast<int>(window.width), static_cast<int>(window.height), window.pTitle, nullptr, nullptr);
		if (s_pWindow == nullptr) {
			return false;
		}

		glfwSetWindowSizeCallback(s_pWindow, windowSizeCallback);
		glfwSetWindowCloseCallback(s_pWindow, windowCloseCallback);

		return true;
	}

	void TINY_ENGINE_APICALL WindowSystem::shutdown()
	{
		glfwDestroyWindow(s_pWindow);
		s_pWindow = nullptr;
	}

	bool TINY_ENGINE_APICALL WindowSystem::update()
	{
		glfwPollEvents();
		return !glfwWindowShouldClose(s_pWindow);
	}

	void TINY_ENGINE_APICALL WindowSystem::setWindowSize(WindowSize const& size)
	{
		glfwSetWindowSize(s_pWindow, static_cast<int>(size.width), static_cast<int>(size.height));
	}

	WindowSize TINY_ENGINE_APICALL WindowSystem::getWindowSize() const
	{
		int width = 0, height = 0;
		glfwGetWindowSize(s_pWindow, &width, &height);

		return WindowSize{ static_cast<uint32_t>(width), static_cast<uint32_t>(height) };
	}

	void TINY_ENGINE_APICALL WindowSystem::setResizeHandler(ResizeHandler const& handler)
	{
		s_ResizeHandler = handler;
	}

	void TINY_ENGINE_APICALL WindowSystem::setCloseHandler(CloseHandler const& handler)
	{
		s_CloseHandler = handler;
	}

	bool TINY_ENGINE_APICALL WindowSystem::minimized() const
	{
		return s_WindowMinimized;
	}

#if		TINY_ENGINE_PLATFORM_WINDOWS
	HWND TINY_ENGINE_APICALL WindowSystem::getHWND() const
	{
		if (s_pWindow == nullptr) {
			return nullptr;
		}

		return glfwGetWin32Window(s_pWindow);
	}
#endif

#if		TINY_ENGINE_VULKAN_AVAILABLE
	char const** TINY_ENGINE_APICALL WindowSystem::requiredVulkanInstanceExtensions(uint32_t* pExtensionCount) const
	{
		return glfwGetRequiredInstanceExtensions(pExtensionCount);
	}

	VkResult TINY_ENGINE_APICALL WindowSystem::createVulkanWindowSurface(VkInstance instance, VkAllocationCallbacks const* pAllocator, VkSurfaceKHR* pSurface)
	{
		return glfwCreateWindowSurface(instance, s_pWindow, pAllocator, pSurface);
	}
#endif
} // namespace tiny_engine::core
