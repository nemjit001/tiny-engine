#pragma once

#include <string>
#include <vector>

namespace tiny_engine::core
{
	/// @brief Commandline argument store, parsed from the `argv` passed to main.
	class CommandlineArgs
	{
	public:
		static constexpr char const* const EndOfOptions = "--";

		CommandlineArgs(int argc, char const** argv);

		/// @brief Check if a commandline variable is set.
		/// @param name Entire name of the variable, with leading symbols (i.e. "foo" is different from "--foo").
		/// @return A boolean indicating if the variable is set.
		bool isSet(std::string const& name) const;

		/// @brief Retrieve the value associated with a commandline argument.
		/// @param name Entire name of the variable, with leading symbols.
		/// @return An empty string if not set, or value is missing.
		std::string argValue(std::string const& name) const;

		std::vector<std::string> getPostOptionsValues() const;

	private:
		std::vector<std::string> m_argv;
	};
} // namespace tiny_engine::core
