#pragma once

#include <cstdio>
#include <cstdlib>

#include "core/commandline_args.hpp"

/// @brief Abort the current process, logging the abort msg.
/// @param msg Message logged to stderr on process abort.
#define TINY_ENGINE_DIE(msg)	(void)(fprintf(stderr, "%s\n", msg), abort())

namespace tiny_engine
{
	class Application;

	/// @brief The Engine class wraps engine system lifetime.
	class Engine
	{
	public:
		/// @brief 
		/// @param args Parsed commandline arguments passed to entrypoint function.
		Engine(CommandlineArgs const& args);
		~Engine();

		Engine(Engine const&) = delete;
		Engine& operator=(Engine const&) = delete;

		/// @brief Run the engine main loop.
		/// @param pApplication Application running on top of the engine.
		/// @return 0 on success, non-zero otherwise.
		int run(Application* pApplication);

	private:
		CommandlineArgs m_args;
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
