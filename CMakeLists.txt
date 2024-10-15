cmake_minimum_required(VERSION 3.21)
project("Tiny Engine")
enable_testing()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(TINY_ENGINE_BUILD_TESTS "Build Tiny Engine test suite" ON)

if (WIN32)
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/")
	set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib/")
	set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib/")
endif()

function(target_warnings_as_errors TARGET_NAME)
	if (MSVC)
		target_compile_options(${TARGET_NAME} PRIVATE /W4 /WX)
	else()
		target_compile_options(${TARGET_NAME} PRIVATE -Wall -Wextra -Wpedantic -Werror)
	endif()
endfunction()

include("vendored/vendored.cmake")

add_subdirectory("tiny-engine/")
add_subdirectory("tiny-renderer/")
