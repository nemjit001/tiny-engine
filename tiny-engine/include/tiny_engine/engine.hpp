#pragma once

#include "tiny_engine/core/commandline_args.hpp"
#include "tiny_engine/core/window_system.hpp"

namespace tiny_engine
{
	class Application;

	/// @brief The EngineResult enum gives error codes returned by Engine::run a readable name,
	//	and groups enum values by Engine and App errors.
	enum EngineResult
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
		Engine(CommandlineArgs const& args);
		~Engine() = default;

		Engine(Engine const&) = delete;
		Engine& operator=(Engine const&) = delete;

		/// @brief Run the engine main loop.
		/// @param pApplication Application running on top of the engine.
		/// @return 0 on success, non-zero otherwise.
		int run(Application* pApplication);

	private:
		CommandlineArgs m_args;
		WindowSystem m_windowSystem;
	};
} // namespace tiny_engine

/// @brief Generate the fixed program entrypoint code for an Application child class.
/// @param AppClass Application child class to run on top of the tiny_engine::Engine.
#define TINY_ENGINE_MAKE_ENTRYPOINT(AppClass)				\
	int main(int argc, char const** argv)					\
	{														\
		tiny_engine::CommandlineArgs args(argc, argv);		\
		AppClass app;										\
		return tiny_engine::Engine(args).run(&app);			\
	}
