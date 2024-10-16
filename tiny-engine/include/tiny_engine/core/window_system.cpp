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

namespace tiny_engine::core
{
	namespace
	{
		bool s_WindowMinimized = false;
		GLFWwindow* s_pWindow = nullptr;

		void windowSizeCallback(GLFWwindow* pWindow, int width, int height)
		{
			if (pWindow != s_pWindow) {
				return;
			}

			s_WindowMinimized = (width == 0 || height == 0);
			WindowSystem* pThis = reinterpret_cast<WindowSystem*>(glfwGetWindowUserPointer(pWindow));
			if (!s_WindowMinimized) {
				pThis->handleResize(WindowSize{ static_cast<uint32_t>(width), static_cast<uint32_t>(height) });
			}
		}

		void windowCloseCallback(GLFWwindow* pWindow)
		{
			if (pWindow != s_pWindow) {
				return;
			}

			WindowSystem* pThis = reinterpret_cast<WindowSystem*>(glfwGetWindowUserPointer(pWindow));
			pThis->handleClose();
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

	bool WindowSystem::init(WindowDesc const& window)
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

		glfwSetWindowUserPointer(s_pWindow, this);
		glfwSetWindowSizeCallback(s_pWindow, windowSizeCallback);
		glfwSetWindowCloseCallback(s_pWindow, windowCloseCallback);

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

	void WindowSystem::setWindowSize(WindowSize const& size)
	{
		glfwSetWindowSize(s_pWindow, static_cast<int>(size.width), static_cast<int>(size.height));
	}

	void WindowSystem::setResizeHandler(ResizeHandler const& handler)
	{
		m_resizeHandler = handler;
	}

	void WindowSystem::setCloseHandler(CloseHandler const& handler)
	{
		m_closeHandler = handler;
	}

	bool WindowSystem::minimized() const
	{
		return s_WindowMinimized;
	}

	void WindowSystem::handleResize(WindowSize const& size) const
	{
		if (m_resizeHandler) {
			m_resizeHandler(size);
		}
	}

	void WindowSystem::handleClose() const
	{
		if (m_closeHandler) {
			m_closeHandler();
		}
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
} // namespace tiny_engine::core
