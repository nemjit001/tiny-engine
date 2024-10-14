project("Tiny Engine" VERSION 0.1.0)

find_package(Vulkan)

configure_file(engine_config.h.in engine_config.h)

file(GLOB_RECURSE TINY_ENGINE_PUBLIC_SOURCES CONFIGURE_DEPENDS "include/*.cpp" "include/*.hpp")
file(GLOB_RECURSE TINY_ENGINE_PRIVATE_SOURCES CONFIGURE_DEPENDS "src/*.cpp" "src/*.hpp")

add_library(TinyEngine STATIC ${TINY_ENGINE_PUBLIC_SOURCES} ${TINY_ENGINE_PRIVATE_SOURCES})
target_link_libraries(TinyEngine PUBLIC glfw ${GLFW_LIBRARIES})
target_compile_definitions(TinyEngine PUBLIC GLFW_INCLUDE_NONE)
target_include_directories(TinyEngine PUBLIC "include/" "${CMAKE_CURRENT_BINARY_DIR}" PRIVATE "src/")
target_warnings_as_errors(TinyEngine)

if (${VULKAN_FOUND})
	message(STATUS "TinyEngine: Found Vulkan")
	target_link_libraries(TinyEngine PUBLIC Vulkan::Vulkan)
	target_compile_definitions(TinyEngine PUBLIC TINY_ENGINE_VULKAN_AVAILABLE=1)
endif()

if (${TINY_ENGINE_BUILD_TESTS})
	add_subdirectory("test/")
endif()
