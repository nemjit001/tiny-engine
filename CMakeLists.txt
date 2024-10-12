cmake_minimum_required(VERSION 3.14)
project("Tiny Engine")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include("vendored/vendored.cmake")

add_subdirectory("tiny-engine/")
add_subdirectory("tiny-renderer/")

add_custom_target(TinyEngineCopyDataFolder ALL
	COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/data/" "${CMAKE_CURRENT_BINARY_DIR}/data/"
)
