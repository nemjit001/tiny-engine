#pragma once

#include <cstdint>

namespace tiny_engine::core
{
	enum class LogLevel : uint8_t
	{
		Trace = 0x01,
		Info = 0x02,
		Warning = 0x04,
		Error = 0x08,

		None = 0x00,
		All = Trace | Info | Warning | Error,
	};

	class Logging
	{
	public:
		static char const* LevelToString(LogLevel level);

		template<typename ...Args>
		static void Log(LogLevel level, char const* fmt, Args&&... args);
	};
} // namespace tiny_engine::core

#include "tiny_engine/core/logging.inl"
