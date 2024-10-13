cmake_minimum_required(VERSION 3.14)
project("Tiny Engine")
enable_testing()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(TINY_ENGINE_BUILD_TESTS "Build Tiny Engine test suite" ON)

include("vendored/vendored.cmake")

function(target_warnings_as_errors TARGET_NAME)
	if (MSVC)
		target_compile_options(${TARGET_NAME} PRIVATE /W4 /WX)
	else()
		target_compile_options(${TARGET_NAME} PRIVATE -Wall -Wextra -Wpedantic -Werror)
	endif()
endfunction()

add_subdirectory("tiny-engine/")
add_subdirectory("tiny-renderer/")

add_custom_target(TinyEngineCopyDataFolder ALL
	COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/data/" "${CMAKE_CURRENT_BINARY_DIR}/data/"
)
