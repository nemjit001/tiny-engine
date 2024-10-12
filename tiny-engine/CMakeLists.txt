project("Tiny Engine" VERSION 0.1.0)

configure_file(engine_config.h.in engine_config.h)

file(GLOB_RECURSE TINY_ENGINE_PUBLIC_SOURCES CONFIGURE_DEPENDS "include/*.cpp" "include/*.hpp")
file(GLOB_RECURSE TINY_ENGINE_PRIVATE_SOURCES CONFIGURE_DEPENDS "src/*.cpp" "src/*.hpp")

add_library(TinyEngine STATIC ${TINY_ENGINE_PUBLIC_SOURCES} ${TINY_ENGINE_PRIVATE_SOURCES})
target_link_libraries(TinyEngine PUBLIC glfw ${GLFW_LIBRARIES})
target_compile_definitions(TinyEngine PUBLIC GLFW_INCLUDE_NONE)
target_include_directories(TinyEngine PUBLIC "include/" PRIVATE "src/" "${CMAKE_CURRENT_BINARY_DIR}")
target_warnings_as_errors(TinyEngine)
