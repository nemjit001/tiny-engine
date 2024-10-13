#pragma once

#define TINY_ENGINE_BUILD_DEBUG			(!NDEBUG)
#define TINY_ENGINE_BUILD_RELEASE		(!TINY_ENGINE_BUILD_DEBUG)

#define TINY_ENGINE_PLATFORM_WINDOWS	(_WIN32 > 0L)
#define TINY_ENGINE_PLATFORM_LINUX		(__unix > 0L)

#define TINY_ENGINE_COMPILER_CLANG		(__clang__ > 0L)
#define TINY_ENGINE_COMPILER_MSVC		((_MSC_VER > 0L) && !(TINY_ENGINE_COMPILER_CLANG))
#define TINY_ENGINE_COMPILER_GCC		((__GNUC__ > 0L) && !(TINY_ENGINE_COMPILER_CLANG))

// Win32 defines for windows api exclusion
#if		TINY_ENGINE_PLATFORM_WINDOWS

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#ifndef NOMINMAX
#define NOMINMAX
#endif

#endif

// Auto generated config file included in defines
#include "engine_config.h"
