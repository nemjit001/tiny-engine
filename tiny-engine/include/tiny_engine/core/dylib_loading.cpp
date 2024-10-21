#include "tiny_engine/core/dylib_loading.hpp"

#if		TINY_ENGINE_PLATFORM_WINDOWS
	#include <windows.h>
#elif	TINY_ENGINE_PLATFORM_LINUX
	#include <dlfcn.h>
#endif

namespace tiny_engine::core
{
#if		TINY_ENGINE_PLATFORM_WINDOWS
	using PlatformHandle = HMODULE;
#elif	TINY_ENGINE_PLATFORM_LINUX
	using PlatformHandle = void*;
#else
	using PlatformHandle = void*;
#endif

	struct DyLibHandle
	{
		PlatformHandle handle;
	};

	std::string TINY_ENGINE_APICALL DyLib::makeLibName(std::string const& name)
	{
		return TINY_ENGINE_DYLIB_PREFIX + name + TINY_ENGINE_DYLIB_POSTFIX;
	}

	DyLibHandle* TINY_ENGINE_APICALL DyLib::open(std::string const& name)
	{
		PlatformHandle handle = nullptr;
#if		TINY_ENGINE_PLATFORM_WINDOWS
		UINT const loadFlags = 0;
		handle = LoadLibraryEx(name.c_str(), nullptr, loadFlags);
#elif	TINY_ENGINE_PLATFORM_LINUX
		int const mode = 0;
		handle = dlopen(name.c_str(), mode);
#endif

		return new DyLibHandle{ handle };
	}

	void TINY_ENGINE_APICALL DyLib::close(DyLibHandle* pLibrary)
	{
		if (pLibrary == nullptr || pLibrary->handle == nullptr) {
			return;
		}

#if		TINY_ENGINE_PLATFORM_WINDOWS
		FreeLibrary(pLibrary->handle);
#elif	TINY_ENGINE_PLATFORM_LINUX
		dlclose(pLibrary->handle);
#endif

		delete pLibrary;
	}

	PFN_TinyEngineVoidFunc TINY_ENGINE_APICALL DyLib::getProcAddr(DyLibHandle* pLibrary, char const* pProcName)
	{
		if (pLibrary == nullptr || pLibrary->handle == nullptr) {
			return nullptr;
		}

		PFN_TinyEngineVoidFunc pFunc = nullptr;
#if		TINY_ENGINE_PLATFORM_WINDOWS
		pFunc = reinterpret_cast<PFN_TinyEngineVoidFunc>(GetProcAddress(pLibrary->handle, pProcName));
#elif	TINY_ENGINE_PLATFORM_LINUX
		pFunc = reinterpret_cast<PFN_TinyEngineVoidFunc>(dlsym(pLibrary->handle, pProcName));
#endif

		return pFunc;
	}
} // namespace tiny_engine::core
