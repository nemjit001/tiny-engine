#include "tiny_engine/core/dylib_loading.hpp"

#if		TINY_ENGINE_PLATFORM_WINDOWS
	#include <windows.h>
#elif	TINY_ENGINE_PLATFORM_LINUX
	#include <dlfcn.h>
#endif

#include "tiny_engine/assert.hpp"

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

	DyLibHandle* TINY_ENGINE_APICALL DyLib::open(char const* pName)
	{
		TINY_ENGINE_ASSERT(pName != nullptr);

		PlatformHandle handle = nullptr;
#if		TINY_ENGINE_PLATFORM_WINDOWS
		handle = LoadLibraryEx(pName, nullptr, 0);
#elif	TINY_ENGINE_PLATFORM_LINUX
		handle = dlopen(pName, RTLD_NOW);
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

	bool TINY_ENGINE_APICALL DyLib::loaded(DyLibHandle* pHandle)
	{
		if (pHandle == nullptr) {
			return false;
		}

		return pHandle->handle != nullptr;
	}
} // namespace tiny_engine::core
