#include "tiny_engine/core/window_system.hpp"

#include <GLFW/glfw3.h>

#include "tiny_engine/defines.hpp"
#include "tiny_engine/error_handling.hpp"

#if TINY_ENGINE_PLATFORM_WINDOWS
	#define GLFW_EXPOSE_NATIVE_WIN32
	#include <GLFW/glfw3native.h>
#endif

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
} // namespace tiny_engine
