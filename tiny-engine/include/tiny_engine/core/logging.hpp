#pragma once

#include <cstdint>

#include "tiny_engine/defines.hpp"

namespace tiny_engine::core
{
	/// @brief Log level for use in Logging system.
	enum class LogLevel : int8_t
	{
		Trace = 0x01,
		Info = 0x02,
		Warning = 0x04,
		Error = 0x08,

		None = 0x00,
		All = Trace | Info | Warning | Error,
	};

	class TINY_ENGINE_API Logging
	{
	public:
		/// @brief Convert a given LogLevel to its cstring represenation.
		/// @param level 
		/// @return A static lifetime cstring.
		static char const* TINY_ENGINE_APICALL LevelToString(LogLevel level);

		/// @brief Log a message using the internal logging mechanism.
		/// @tparam ...Args Argument pack to be passed to the log function.
		/// @param level Log level for filtering.
		/// @param fmt Format string using printf conventions.
		/// @param ...args Arguments for the format string.
		template<typename ...Args>
		static void TINY_ENGINE_APICALL Log(LogLevel level, char const* fmt, Args&&... args);
	};
} // namespace tiny_engine::core

#include "tiny_engine/core/logging.inl"
