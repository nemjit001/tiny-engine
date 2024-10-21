#pragma once

#define TINY_ENGINE_BUILD_DEBUG			(!NDEBUG)
#define TINY_ENGINE_BUILD_RELEASE		(!TINY_ENGINE_BUILD_DEBUG)

#define TINY_ENGINE_PLATFORM_WINDOWS	(_WIN32 > 0L)
#define TINY_ENGINE_PLATFORM_LINUX		(__unix > 0L)

#define TINY_ENGINE_COMPILER_CLANG		(__clang__ > 0L)
#define TINY_ENGINE_COMPILER_MSVC		((_MSC_VER > 0L) && !(TINY_ENGINE_COMPILER_CLANG))
#define TINY_ENGINE_COMPILER_GCC		((__GNUC__ > 0L) && !(TINY_ENGINE_COMPILER_CLANG))

#if			TINY_ENGINE_COMPILER_MSVC
	#define TINY_ENGINE_NODISCARD	[[nodiscard]]
	#define TINY_ENGINE_UNUSED		[[maybe_unused]]
	#define TINY_ENGINE_FORCEINLINE	__forceinline
	#define TINY_ENGINE_NOINLINE	__declspec(noinline)
	#define TINY_ENGINE_APICALL		__stdcall
	#if		TINY_ENGINE_EXPORT_SYMBOLS
		#define TINY_ENGINE_API		__declspec(dllexport)
	#else
		#define TINY_ENGINE_API		__declspec(dllimport)
	#endif
#elif		TINY_ENGINE_COMPILER_CLANG || TINY_ENGINE_COMPILER_GCC
	#define TINY_ENGINE_NODISCARD	[[nodiscard]] __attribute__((warn_unused_result))
	#define TINY_ENGINE_UNUSED		[[maybe_unused]] __attribute__((unused))
	#define TINY_ENGINE_FORCEINLINE	__attribute__((always_inline)) inline
	#define TINY_ENGINE_NOINLINE	__attribute__((noinline))
	#define TINY_ENGINE_APICALL		__attribute__((stdcall))
	#if		TINY_ENGINE_EXPORT_SYMBOLS
		#define TINY_ENGINE_API		__attribute__((visibility("default")))
	#else
		#define TINY_ENGINE_API
	#endif
#else
	#define TINY_ENGINE_NODISCARD	[[nodiscard]]
	#define TINY_ENGINE_UNUSED		[[maybe_unused]]
	#define TINY_ENGINE_FORCEINLINE	inline
	#define TINY_ENGINE_NOINLINE
	#define TINY_ENGINE_APICALL
	#define TINY_ENGINE_API
#endif

// NOLINTBEGIN(bugprone-reserved-identifier)
#define _TINY_ENGINE_STRINGIFY(var)		#var
#define _TINY_ENGINE_CONCAT(lhs, rhs)	lhs##rhs
// NOLINTEND(bugprone-reserved-identifier)

#define TINY_ENGINE_MARK_UNUSED(var)	((void)(var))
#define TINY_ENGINE_SIZEOF_ARRAY(arr)	(sizeof((arr)) / sizeof((arr)[0]))

#define TINY_ENGINE_EXPAND(macro)		macro
#define TINY_ENGINE_STRINGIFY(var)		_TINY_ENGINE_STRINGIFY_SIMPLE(var)
#define TINY_ENGINE_CONCAT(lhs, rhs)	_TINY_ENGINE_CONCAT_SIMPLE(lhs, rhs)

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
