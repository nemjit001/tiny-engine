#pragma once

#include <cstddef>
#include <cstdint>

#include "tiny_engine/defines.hpp"

namespace tiny_engine::core
{
	/// @brief Commandline argument store, parsed from the `argv` passed to main.
	class TINY_ENGINE_API CommandlineArgs
	{
	public:
		CommandlineArgs(int argc, char const** argv);

		/// @brief Check if a commandline variable is set.
		/// @param pName Entire name of the variable, with leading symbols (i.e. "foo" is different from "--foo").
		/// @return A boolean indicating if the variable is set.
		bool TINY_ENGINE_APICALL isSet(char const* pName) const;

		/// @brief Retrieve the value associated with a commandline argument.
		/// @param pName Entire name of the variable, with leading symbols.
		/// @return An empty cstring if not set, or the value is missing.
		char const* TINY_ENGINE_APICALL argValue(char const* pName) const;

		/// @brief Get the post options values passed to the program.
		/// @param pCount Number of option values returned.
		/// @return 
		char const** TINY_ENGINE_APICALL getPostOptionsValues(uint32_t* pCount) const;

	private:
		static constexpr char const* const EndOfOptions = "--";

		size_t m_argc = 0;
		char const** m_ppArgv = nullptr;
	};
} // namespace tiny_engine::core
