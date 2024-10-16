#pragma once

#include <cstdint>

#include "tiny_engine/core/commandline_args.hpp"
#include "tiny_engine/core/renderer.hpp"
#include "tiny_engine/core/window_system.hpp"

namespace tiny_engine
{
	class IApplication;

	/// @brief The EngineResult enum gives error codes returned by Engine::run a readable name,
	//	and groups enum values by Engine and App errors.
	enum EngineResult : int32_t
	{
		eEngineResultOK = 0,

		eEngineResultSubsystemCodes = 0x10,
		eEngineResultSubsystemInitFailed = eEngineResultSubsystemCodes,

		eEngineResultAppCodes = 0x20,
		eEngineResultNoApp = eEngineResultAppCodes,
		eEngineResultAppInitFailed,
	};

	/// @brief The Engine class contains engine functionality (init, shutdown, main loop).
	class Engine
	{
	public:
		/// @brief 
		/// @param args Parsed commandline arguments passed to entrypoint function.
		explicit Engine(core::CommandlineArgs const& args);
		~Engine() = default;

		Engine(Engine const&) noexcept = delete;
		Engine& operator=(Engine const&) noexcept = delete;

		/// @brief Run the engine main loop.
		/// @param pApplication Application running on top of the engine.
		/// @return 0 on success, non-zero otherwise.
		int run(IApplication* pApplication);

	private:
		core::CommandlineArgs m_args;
		core::WindowSystem m_windowSystem;
		core::Renderer m_renderer;
	};
} // namespace tiny_engine

/// @brief Generate the fixed program entrypoint code for an Application child class.
/// @param AppClass Application child class to run on top of the tiny_engine::Engine.
#define TINY_ENGINE_MAKE_ENTRYPOINT(AppClass)					\
	int main(int argc, char const** argv)						\
	{															\
		tiny_engine::core::CommandlineArgs args(argc, argv);	\
		AppClass app;											\
		return tiny_engine::Engine(args).run(&app);				\
	}
