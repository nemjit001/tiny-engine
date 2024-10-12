project("Tiny Renderer" VERSION 0.1.0)

configure_file(app_config.h.in app_config.h)

file(GLOB_RECURSE TINY_RENDERER_SOURCES CONFIGURE_DEPENDS "src/*.cpp" "src/*.hpp")

add_executable(TinyRenderer ${TINY_RENDERER_SOURCES})
target_link_libraries(TinyRenderer PRIVATE TinyEngine)
target_compile_definitions(TinyRenderer PRIVATE)
target_include_directories(TinyRenderer PRIVATE "src/" "${CMAKE_CURRENT_BINARY_DIR}")
target_warnings_as_errors(TinyRenderer)
