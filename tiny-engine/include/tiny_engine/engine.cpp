#include "tiny_engine/engine.hpp"

#include <cstdio>

#include "tiny_engine/defines.hpp"
#include "tiny_engine/application.hpp"
#include "tiny_engine/core/commandline_args.hpp"
#include "tiny_engine/core/window_system.hpp"

namespace tiny_engine
{
	static constexpr uint32_t const DefaultWindowWidth = 1600;
	static constexpr uint32_t const DefaultWindowHeight = 900;
	static constexpr bool const DefaultWindowResizable = true;

	Engine::Engine(CommandlineArgs const& args)
		:
		m_args(args),
		m_windowSystem()
	{
		//
	}

	int Engine::run(Application* pApplication)
	{
		if (pApplication == nullptr) {
			return eEngineResultNoApp;
		}

		WindowDesc window{};
		window.pTitle = pApplication->name();
		window.width = DefaultWindowWidth;
		window.height = DefaultWindowHeight;
		window.resizable = DefaultWindowResizable;

		if (!m_windowSystem.init(window)) {
			return eEngineResultSubsystemInitFailed;
		}

		if (!pApplication->init(m_args)) {
			return eEngineResultAppInitFailed;
		}

		// TODO(nemjit001): Replace printf by internal logging mechanism
		char const* pAppName = pApplication->name();
		AppVersion const appVersion = pApplication->version();
		printf("Initialized %s (%s)\n", TINY_ENGINE_NAME, TINY_ENGINE_VERSION_STRING);
		printf("Loaded application: %s (v%d.%d.%d)\n", pAppName, appVersion.major, appVersion.minor, appVersion.patch);

		// TODO(nemjit001): notify subsystems that require knowledge of resize in handler.
		m_windowSystem.setResizeHandler([&](uint32_t width, uint32_t height) {
			printf("Resize: %u %u\n", width, height);
		});

		bool running = true;
		m_windowSystem.setCloseHandler([&running]() {
			running = false;
		});

		while (running)
		{
			if (!m_windowSystem.update() && running) {
				printf("Failed to update window system\n");
				running = false;
				continue;
			}

			if (!m_windowSystem.minimized()) {
				if (!pApplication->update()) {
					printf("Failed to update application state\n");
					running = false;
					continue;
				}

				pApplication->render();
			}
		}

		pApplication->shutdown();
		m_windowSystem.shutdown();
		return eEngineResultOK;
	}
}
