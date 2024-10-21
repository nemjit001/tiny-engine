#include "tiny_engine/engine.hpp"

#include "engine_config.h"
#include "tiny_engine/application.hpp"
#include "tiny_engine/core/commandline_args.hpp"
#include "tiny_engine/core/logging.hpp"
#include "tiny_engine/core/window_system.hpp"

namespace tiny_engine
{
	static constexpr uint32_t const DefaultWindowWidth = 1600;
	static constexpr uint32_t const DefaultWindowHeight = 900;

	Engine::Engine(core::CommandlineArgs const& args)
		:
		m_args(args),
		m_windowSystem()
	{
		//
	}

	int Engine::run(IApplication* pApplication)
	{
		if (pApplication == nullptr) {
			return eEngineResultNoApp;
		}

		core::WindowDesc window{};
		window.pTitle = pApplication->name();
		window.width = DefaultWindowWidth;
		window.height = DefaultWindowHeight;
		window.resizable = pApplication->allowWindowResize();

		if (!m_windowSystem.init(window)) {
			return eEngineResultSubsystemInitFailed;
		}

		if (!pApplication->init(m_args, &m_windowSystem)) {
			return eEngineResultAppInitFailed;
		}

		m_windowSystem.setResizeHandler([&](core::WindowSize const& size) {
			pApplication->handleResize(size);
		});

		m_windowSystem.setCloseHandler([&]() {
			pApplication->exit();
		});

		char const* pAppName = pApplication->name();
		AppVersion const appVersion = pApplication->version();
		core::Logging::Log(core::LogLevel::Info, "Initialized %s (%s)\n", TINY_ENGINE_NAME, TINY_ENGINE_VERSION_STRING);
		core::Logging::Log(core::LogLevel::Info, "Loaded application: %s (v%d.%d.%d)\n", pAppName, appVersion.major, appVersion.minor, appVersion.patch);

		while (pApplication->isRunning())
		{
			if (!m_windowSystem.update() && pApplication->isRunning()) {
				core::Logging::Log(core::LogLevel::Error, "Failed to update window system\n");
				pApplication->exit();
			}

			if (!m_windowSystem.minimized()) {
				if (!pApplication->update()) {
					core::Logging::Log(core::LogLevel::Error, "Failed to update application state\n");
					pApplication->exit();
				}

				pApplication->render();
			}
		}

		pApplication->shutdown();
		m_windowSystem.shutdown();
		return eEngineResultOK;
	}
} // namespace tiny_engine
