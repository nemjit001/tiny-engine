#pragma once

#include <string>

#include "tiny_engine/defines.hpp"

#if TINY_ENGINE_PLATFORM_WINDOWS
	#define TINY_ENGINE_DYLIB_PREFIX	""
	#define TINY_ENGINE_DYLIB_POSTFIX	".dll"
#elif TINY_ENGINE_PLATFORM_LINUX
	#define TINY_ENGINE_DYLIB_PREFIX	"lib"
	#define TINY_ENGINE_DYLIB_POSTFIX	".so"
#endif

namespace tiny_engine::core
{
	typedef void(TINY_ENGINE_APICALL *PFN_TinyEngineVoidFunc)(void);

	struct DyLibHandle;

	class TINY_ENGINE_API DyLib
	{
	public:
		/// @brief Open a DyLibHandle using a library name. This must be a platform library name with the pre and postfix values matching the platform conventions.
		/// @param pName Library name.
		/// @return A new DyLibHandle pointer that must be closed using `DyLib::close`.
		static DyLibHandle* TINY_ENGINE_APICALL open(char const* pName);

		/// @brief Close a DyLibHandle, freeing internal resources.
		/// @param pLibrary Library handle to close.
		static void TINY_ENGINE_APICALL close(DyLibHandle* pLibrary);

		/// @brief Retrieve a function address from a library handle, may return nullptr as valid value.
		/// @param pLibrary Library handle to retrieve function pointer from.
		/// @param pProcName Function symbol name.
		/// @return A TinyEngineVoidFunc pointer that may be reinterpreted to the underlying function type.
		static PFN_TinyEngineVoidFunc TINY_ENGINE_APICALL getProcAddr(DyLibHandle* pLibrary, char const* pProcName);

		/// @brief Test if a dynamic library has been loaded.
		/// @param pHandle Handle to test.
		/// @return 
		static bool TINY_ENGINE_APICALL loaded(DyLibHandle* pHandle);
	};
} // namespace tiny_engine::core
