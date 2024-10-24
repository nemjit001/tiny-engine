#pragma once

#define TINY_ENGINE_BUILD_DEBUG			(!NDEBUG)
#define TINY_ENGINE_BUILD_RELEASE		(!TINY_ENGINE_BUILD_DEBUG)

#define TINY_ENGINE_PLATFORM_WINDOWS	(_WIN32 > 0L)
#define TINY_ENGINE_PLATFORM_LINUX		(__unix > 0L)

#define TINY_ENGINE_COMPILER_CLANG		(__clang__ > 0L)
#define TINY_ENGINE_COMPILER_MSVC		(_MSC_VER > 0L)
#define TINY_ENGINE_COMPILER_GCC		(__GNUC__ > 0L)

// Define per compiler function attributes
#if			TINY_ENGINE_COMPILER_MSVC && !TINY_ENGINE_COMPILER_CLANG
	#define TINY_ENGINE_NODISCARD	[[nodiscard]]
	#define TINY_ENGINE_UNUSED		[[maybe_unused]]
	#define TINY_ENGINE_FORCEINLINE	__forceinline
	#define TINY_ENGINE_NOINLINE	__declspec(noinline)
#elif		TINY_ENGINE_COMPILER_GCC || TINY_ENGINE_COMPILER_CLANG
	#define TINY_ENGINE_NODISCARD	[[nodiscard]] __attribute__((warn_unused_result))
	#define TINY_ENGINE_UNUSED		[[maybe_unused]] __attribute__((unused))
	#define TINY_ENGINE_FORCEINLINE	__attribute__((always_inline)) inline
	#define TINY_ENGINE_NOINLINE	__attribute__((noinline))
#else
	#define TINY_ENGINE_NODISCARD	[[nodiscard]]
	#define TINY_ENGINE_UNUSED		[[maybe_unused]]
	#define TINY_ENGINE_FORCEINLINE	inline
	#define TINY_ENGINE_NOINLINE
#endif

// Define per compiler dynamic lib API attributes
#if		TINY_ENGINE_COMPILER_MSVC || (TINY_ENGINE_COMPILER_MSVC && TINY_ENGINE_COMPILER_CLANG)
	#define TINY_ENGINE_APICALL		__stdcall
	#if	TINY_ENGINE_EXPORT_SYMBOLS
		#define TINY_ENGINE_API		__declspec(dllexport)
	#elif	!TINY_ENGINE_BUILD_STATIC
		#define TINY_ENGINE_API		__declspec(dllimport)
	#else
		#define TINY_ENGINE_API
	#endif
#elif	TINY_ENGINE_COMPILER_GCC || (TINY_ENGINE_COMPILER_GCC && TINY_ENGINE_COMPILER_CLANG)
	#define TINY_ENGINE_APICALL
	#if	TINY_ENGINE_EXPORT_SYMBOLS
		#define TINY_ENGINE_API		__attribute__((visibility("default")))
	#else
		#define TINY_ENGINE_API
	#endif
#endif

// NOLINTBEGIN(bugprone-reserved-identifier)
#define _TINY_ENGINE_STRINGIFY(var)			#var
#define _TINY_ENGINE_CONCAT(lhs, rhs)		lhs ## rhs
#define _TINY_ENGINE_STRCONCAT(lhs, rhs)	lhs rhs
// NOLINTEND(bugprone-reserved-identifier)

#define TINY_ENGINE_MARK_UNUSED(var)	((void)(var))
#define TINY_ENGINE_SIZEOF_ARRAY(arr)	(sizeof((arr)) / sizeof((arr)[0]))

#define TINY_ENGINE_EXPAND(macro)		macro
#define TINY_ENGINE_STRINGIFY(var)		_TINY_ENGINE_STRINGIFY(var)
#define TINY_ENGINE_CONCAT(lhs, rhs)	_TINY_ENGINE_CONCAT(lhs, rhs)
#define TINY_ENGINE_STRCONCAT(lhs, rhs)	_TINY_ENGINE_STRCONCAT(lhs, rhs)

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
