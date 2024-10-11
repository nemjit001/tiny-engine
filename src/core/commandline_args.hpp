#pragma once

namespace tiny_engine
{
	/// @brief Commandline argument store, parsed from `argv` passed to main.
	class CommandlineArgs
	{
	public:
		CommandlineArgs(int argc, char const** argv);
	};
} // namespace tiny_engine
