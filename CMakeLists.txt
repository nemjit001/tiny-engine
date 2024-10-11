cmake_minimum_required(VERSION 3.14)
project("Tiny Renderer" VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include("vendored/vendored.cmake")

file(GLOB_RECURSE TINY_RENDERER_SOURCES CONFIGURE_DEPENDS "src/*.cpp" "src/*.hpp")

add_executable(TinyRenderer ${TINY_RENDERER_SOURCES})
target_link_libraries(TinyRenderer PRIVATE glfw ${GLFW_LIBRARIES} imgui vk-bootstrap::vk-bootstrap GPUOpen::VulkanMemoryAllocator Vulkan::Vulkan)
target_compile_definitions(TinyRenderer PRIVATE GLFW_INCLUDE_NONE GLM_FORCE_LEFT_HANDED GLM_FORCE_DEPTH_ZERO_TO_ONE)
target_include_directories(TinyRenderer PRIVATE "src/")

if (MSVC)
	target_compile_options(TinyRenderer PRIVATE /W4 /WX)
else()
	target_compile_options(TinyRenderer PRIVATE -Wall -Wpedantic -Wextra -Werror)
endif()

add_custom_target(TinyRendererCopyData ALL
	COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/data/" "${CMAKE_CURRENT_BINARY_DIR}/data/"
)
