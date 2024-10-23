#include "tiny_engine/core/commandline_args.hpp"

#include <cstring>

#include "tiny_engine/assert.hpp"

namespace tiny_engine::core
{
	CommandlineArgs::CommandlineArgs(int argc, char const** argv)
		:
		m_argc(static_cast<size_t>(argc)),
		m_ppArgv(argv)
	{
		//
	}

	bool TINY_ENGINE_APICALL CommandlineArgs::isSet(char const* pName) const
	{
		for (size_t i = 0; i < m_argc; i++)
		{
			char const* pArg = m_ppArgv[i];
			if (strcmp(pArg, CommandlineArgs::EndOfOptions) == 0) {
				break;
			}

			if (strcmp(pArg, pName) == 0) {
				return true;
			}
		}

		return false;
	}

	char const* TINY_ENGINE_APICALL CommandlineArgs::argValue(char const* pName) const
	{
		bool found = false;
		size_t argIdx = 0;
		for (size_t i = 0; i < m_argc; i++)
		{
			char const* pArg = m_ppArgv[i];
			if (strcmp(pArg, CommandlineArgs::EndOfOptions) == 0) {
				break;
			}

			if (strcmp(pArg, pName) == 0) {
				found = true;
				argIdx = i;
			}
		}

		// Check if next value is within argv bounds, valiation of retrieved value is left to caller
		if (found && (argIdx + 1) < m_argc) {
			return m_ppArgv[argIdx + 1];
		}

		return "";
	}

	char const** TINY_ENGINE_APICALL CommandlineArgs::getPostOptionsValues(uint32_t& count) const
	{
		bool hasOptions = false;
		size_t optionsStart = 0;
		for (size_t i = 0; i < m_argc; i++)
		{
			char const* pArg = m_ppArgv[i];
			if (strcmp(pArg, CommandlineArgs::EndOfOptions) == 0)
			{
				hasOptions = true;
				optionsStart = i + 1;
				break;
			}
		}

		if (hasOptions && optionsStart < m_argc) {
			count = static_cast<uint32_t>(m_argc - optionsStart);
			return &m_ppArgv[optionsStart];
		}

		count = 0;
		return nullptr;
	}
} // namespace tiny_engine::core
