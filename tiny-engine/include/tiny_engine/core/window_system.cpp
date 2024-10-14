#include "tiny_engine/core/window_system.hpp"

#if TINY_ENGINE_PLATFORM_WINDOWS
	#define GLFW_EXPOSE_NATIVE_WIN32
#endif

#if		TINY_ENGINE_VULKAN_AVAILABLE
	#define GLFW_INCLUDE_VULKAN
#endif

#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#include "tiny_engine/defines.hpp"
#include "tiny_engine/error_handling.hpp"

namespace tiny_engine
{
	static bool s_WindowMinimized = false;
	static GLFWwindow* s_pWindow = nullptr;

	static void windowSizeCallback(GLFWwindow* pWindow, int width, int height)
	{
		(void)(pWindow);
		s_WindowMinimized = (width == 0 || height == 0);

		// TODO(nemjit001): Handle window resize in other subsystems
	}

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

	bool WindowSystem::init(WindowDesc const& window)
	{
		if (s_pWindow != nullptr) {
			return false;
		}

		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, window.resizable);
		s_pWindow = glfwCreateWindow(window.width, window.height, window.pTitle, nullptr, nullptr);
		if (s_pWindow == nullptr) {
			return false;
		}

		glfwSetWindowUserPointer(s_pWindow, nullptr); // TODO(nemjit001): Set user pointer to subsystems required by WindowSystems
		glfwSetWindowSizeCallback(s_pWindow, windowSizeCallback);

		return true;
	}

	void WindowSystem::shutdown()
	{
		glfwDestroyWindow(s_pWindow);
		s_pWindow = nullptr;
	}

	bool WindowSystem::update()
	{
		glfwPollEvents();
		return !glfwWindowShouldClose(s_pWindow);
	}

	void setWindowSize(uint32_t width, uint32_t height)
	{
		glfwSetWindowSize(s_pWindow, width, height);
	}

	bool WindowSystem::minimized() const
	{
		return s_WindowMinimized;
	}

#if		TINY_ENGINE_PLATFORM_WINDOWS
	HWND WindowSystem::getHWND() const
	{
		if (s_pWindow == nullptr) {
			return nullptr;
		}

		return glfwGetWin32Window(s_pWindow);
	}
#endif

#if		TINY_ENGINE_VULKAN_AVAILABLE
	char const** WindowSystem::requiredVulkanInstanceExtensions(uint32_t* pExtensionCount) const
	{
		return glfwGetRequiredInstanceExtensions(pExtensionCount);
	}

	VkResult WindowSystem::createVulkanWindowSurface(VkInstance instance, VkAllocationCallbacks const* pAllocator, VkSurfaceKHR* pSurface)
	{
		return glfwCreateWindowSurface(instance, s_pWindow, pAllocator, pSurface);
	}
#endif
} // namespace tiny_engine
