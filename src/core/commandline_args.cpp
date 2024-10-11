#include "core/commandline_args.hpp"

namespace tiny_engine
{
	CommandlineArgs::CommandlineArgs(int argc, char const** argv)
	{
		this->argv.resize(argc);
		for (int i = 0; i < argc; i++)
		{
			char const* pArg = argv[i];
			this->argv[i] = pArg;
		}
	}

	bool CommandlineArgs::isSet(std::string const& name) const
	{
		for (auto const& arg : argv)
		{
			if (arg == name) {
				return true;
			}
		}

		return false;
	}

	std::string CommandlineArgs::argValue(std::string const& name) const
	{
		bool found = false;
		size_t argIdx = 0;
		for (auto const& arg : argv)
		{
			if (arg == name)
			{
				found = true;
				break;
			}

			argIdx++;
		}

		// Check if next value is within argv bounds, valiation of retrieved value is left to caller
		if (found && (argIdx + 1) < argv.size()) {
			return argv[argIdx + 1];
		}

		return {};
	}
} // namespace tiny_engine
