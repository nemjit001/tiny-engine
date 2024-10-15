cmake_minimum_required(VERSION 3.21)
project("Tiny Engine")
enable_testing()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(TINY_ENGINE_BUILD_TESTS "Build Tiny Engine test suite" ON)

find_program(TINY_ENGINE_CLANG_TIDY "clang-tidy")

if (WIN32)
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/")
	set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib/")
	set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib/")
endif()

if (TINY_ENGINE_CLANG_TIDY)
	message(STATUS "${PROJECT_NAME}: Found clang-tidy (${TINY_ENGINE_CLANG_TIDY})")

	set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
	set(TINY_ENGINE_CLANG_TIDY_COMMAND
		"${TINY_ENGINE_CLANG_TIDY}"
		"--checks=-*,bugprone-*,clang-analyzer-*,concurrency-*,google-*,performance-*,"
		"--header-filter=.*"
	)

	message(STATUS "${PROJECT_NAME}: Using clang-tidy command \"${TINY_ENGINE_CLANG_TIDY_COMMAND}\"")
endif()

function(target_warnings_as_errors TARGET_NAME)
	if (MSVC)
		target_compile_options(${TARGET_NAME} PRIVATE /W4 /WX)
	else()
		target_compile_options(${TARGET_NAME} PRIVATE -Wall -Wextra -Wpedantic -Werror)
	endif()
endfunction()

function(target_enable_clang_tidy TARGET_NAME)
	if (TINY_ENGINE_CLANG_TIDY)
		message(STATUS "${PROJECT_NAME}: Enabled clang-tidy for ${TARGET_NAME}")

		set_target_properties(${TARGET_NAME} PROPERTIES
			C_CLANG_TIDY	"${TINY_ENGINE_CLANG_TIDY_COMMAND}"
			CXX_CLANG_TIDY	"${TINY_ENGINE_CLANG_TIDY_COMMAND}"
		)
	endif()
endfunction()

include("vendored/vendored.cmake")

add_subdirectory("tiny-engine/")
add_subdirectory("tiny-renderer/")
