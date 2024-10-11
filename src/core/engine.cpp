#include "core/application.hpp"
#include "core/engine.hpp"

#include <GLFW/glfw3.h>

namespace tiny_engine
{
	static constexpr uint32_t const DefaultWindowWidth = 1600;
	static constexpr uint32_t const DefaultWindowHeight = 900;

	/// @brief This tracks window minimization state through the GLFW callbacks.
	static bool windowMinimized = false;

	static void windowSizeCallback(GLFWwindow* pWindow, int width, int height)
	{
		Application* pApp = reinterpret_cast<Application*>(glfwGetWindowUserPointer(pWindow));
		if (pApp == nullptr) {
			return;
		}

		windowMinimized = (width == 0 || height == 0);
		if (!windowMinimized) {
			pApp->handleResize(static_cast<uint32_t>(width), static_cast<uint32_t>(height));
		}
	}

	Engine::Engine(CommandlineArgs const& args)
		:
		m_args(args)
	{
		glfwInit();
	}

	Engine::~Engine()
	{
		glfwTerminate();
	}

	int Engine::run(Application* pApplication)
	{
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
		GLFWwindow* pWindow = glfwCreateWindow(DefaultWindowWidth, DefaultWindowHeight, pApplication->name(), nullptr, nullptr);
		if (pWindow == nullptr) {
			return 1;
		}

		if (pApplication == nullptr || !pApplication->init()) {
			return 1;
		}

		glfwSetWindowUserPointer(pWindow, pApplication);
		glfwSetWindowSizeCallback(pWindow, windowSizeCallback);

		bool running = true;
		while (running)
		{
			glfwPollEvents();
			if (glfwWindowShouldClose(pWindow)) {
				running = false;
			}

			if (!windowMinimized) {
				if (!pApplication->update()) {
					running = false;
				}

				pApplication->render();
			}			
		}

		pApplication->shutdown();
		glfwDestroyWindow(pWindow);
		return 0;
	}
}
