#include "tiny_engine/core/logging.hpp"

#include "tiny_engine/error_handling.hpp"

namespace tiny_engine::core
{
	char const* TINY_ENGINE_APICALL Logging::LevelToString(LogLevel level)
	{
		switch (level)
		{
		case LogLevel::Trace:
			return "Trace";
		case LogLevel::Info:
			return "Info";
		case LogLevel::Warning:
			return "Warning";
		case LogLevel::Error:
			return "Error";
		case LogLevel::None:
			return "None";
		case LogLevel::All:
			return "All";
		default:
			break;
		}

		TINY_ENGINE_DIE("Invalid LogLevel encountered");
	}
} // namespace tiny_engine::core
