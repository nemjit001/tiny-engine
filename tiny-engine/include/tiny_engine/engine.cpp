#include "tiny_engine/engine.hpp"

#include <cstdio>

#include <GLFW/glfw3.h>

#include "tiny_engine/application.hpp"
#include "tiny_engine/error_handling.hpp"
#include "tiny_engine/renderer.hpp"
#include "tiny_engine/core/commandline_args.hpp"

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
		if (glfwInit() != GLFW_TRUE) {
			TINY_ENGINE_DIE("Failed to initialize GLFW");
		}
	}

	Engine::~Engine()
	{
		glfwTerminate();
	}

	int Engine::run(Application* pApplication)
	{
		if (pApplication == nullptr) {
			return 1;
		}

		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
		GLFWwindow* pWindow = glfwCreateWindow(DefaultWindowWidth, DefaultWindowHeight, pApplication->name(), nullptr, nullptr);
		if (pWindow == nullptr) {
			return 1;
		}

		if (!pApplication->init(m_args)) {
			return 1;
		}

		char const* pAppName = pApplication->name();
		AppVersion const appVersion = pApplication->version();
		printf("Loaded application: %s (v%d.%d.%d)\n", pAppName, appVersion.major, appVersion.minor, appVersion.patch);

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
