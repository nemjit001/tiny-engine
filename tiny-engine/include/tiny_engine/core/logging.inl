#pragma once

#include "tiny_engine/core/logging.hpp"

#include <cstdio>

namespace tiny_engine::core
{
	template<typename ...Args>
	void TINY_ENGINE_APICALL Logging::Log(LogLevel level, char const* fmt, Args&&... args)
	{
		/* Format security is disabled for this function, since it relies on passing in a format string and a variable number of arguments.
		 * Stream based output might solve this issue, but since C IO is used currently the warning remains disabled.
		 */
#if		TINY_ENGINE_COMPILER_CLANG
	#pragma clang diagnostic push
	#pragma clang diagnostic ignored "-Wformat-security"
#elif 	TINY_ENGINE_COMPILER_GCC
	#pragma GCC diagnostic push
	#pragma GCC diagnostic ignored "-Wformat-security"
#endif

		fprintf(stdout, "[%s]", Logging::LevelToString(level));
		fprintf(stdout, " ");
		fprintf(stdout, fmt, args...);

#if		TINY_ENGINE_COMPILER_CLANG
	#pragma clang diagnostic pop
#elif 	TINY_ENGINE_COMPILER_GCC
	#pragma GCC diagnostic pop
#endif
	}
} // namespace tiny_engine::core
