#include "tiny_engine/core/commandline_args.hpp"

namespace tiny_engine::core
{
	CommandlineArgs::CommandlineArgs(int argc, char const** argv)
	{
		m_argv.resize(argc);
		for (int i = 0; i < argc; i++) {
			m_argv[i] = std::string(argv[i]);
		}
	}

	bool TINY_ENGINE_APICALL CommandlineArgs::isSet(std::string const& name) const
	{
		for (auto const& arg : m_argv)
		{
			if (arg == CommandlineArgs::EndOfOptions) {
				break;
			}

			if (arg == name) {
				return true;
			}
		}

		return false;
	}

	std::string TINY_ENGINE_APICALL CommandlineArgs::argValue(std::string const& name) const
	{
		bool found = false;
		size_t argIdx = 0;
		for (auto const& arg : m_argv)
		{
			if (arg == CommandlineArgs::EndOfOptions) {
				break;
			}

			if (arg == name)
			{
				found = true;
				break;
			}

			argIdx++;
		}

		// Check if next value is within argv bounds, valiation of retrieved value is left to caller
		if (found && (argIdx + 1) < m_argv.size()) {
			return m_argv[argIdx + 1];
		}

		return {};
	}

	std::vector<std::string> TINY_ENGINE_APICALL CommandlineArgs::getPostOptionsValues() const
	{
		std::vector<std::string> values;
		bool postOptions = false;
		for (auto const& arg : m_argv)
		{
			if (arg == CommandlineArgs::EndOfOptions)
			{
				postOptions = true;
				continue;
			}

			if (postOptions) {
				values.push_back(arg);
			}
		}

		return values;
	}
} // namespace tiny_engine::core
