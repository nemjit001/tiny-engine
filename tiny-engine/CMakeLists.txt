project("Tiny Engine" VERSION 0.1.0)

configure_file(engine_config.h.in engine_config.h)

file(GLOB_RECURSE TINY_ENGINE_PUBLIC_SOURCES CONFIGURE_DEPENDS "include/*.cpp" "include/*.hpp" "include/*.inl")
file(GLOB_RECURSE TINY_ENGINE_PRIVATE_SOURCES CONFIGURE_DEPENDS "src/*.cpp" "src/*.hpp" "src/*.inl")

set(TINY_ENGINE_LIBRARIES )

add_library(TinyEngine-static STATIC ${TINY_ENGINE_PUBLIC_SOURCES} ${TINY_ENGINE_PRIVATE_SOURCES})
target_link_libraries(TinyEngine-static PUBLIC glfw ${GLFW_LIBRARIES} Vulkan::Headers Microsoft::DirectX-Headers volk::volk_headers)
target_compile_definitions(TinyEngine-static
	PUBLIC
	TINY_ENGINE_BUILD_STATIC=1
	TINY_ENGINE_EXPORT_SYMBOLS=0
	TINY_ENGINE_VULKAN_AVAILABLE=1
	GLFW_INCLUDE_NONE
	VK_NO_PROTOTYPES
)
target_include_directories(TinyEngine-static PUBLIC "include/" "${CMAKE_CURRENT_BINARY_DIR}" PRIVATE "src/")
target_warnings_as_errors(TinyEngine-static)
target_enable_clang_tidy(TinyEngine-static)

add_library(TinyEngine-shared SHARED ${TINY_ENGINE_PUBLIC_SOURCES} ${TINY_ENGINE_PRIVATE_SOURCES})
target_link_libraries(TinyEngine-shared PUBLIC glfw ${GLFW_LIBRARIES} Vulkan::Headers Microsoft::DirectX-Headers volk::volk_headers)
target_compile_definitions(TinyEngine-shared
	PUBLIC
	TINY_ENGINE_BUILD_STATIC=0
	TINY_ENGINE_VULKAN_AVAILABLE=1
	GLFW_INCLUDE_NONE
	VK_NO_PROTOTYPES
	PRIVATE
	TINY_ENGINE_EXPORT_SYMBOLS=1
)
target_include_directories(TinyEngine-shared PUBLIC "include/" "${CMAKE_CURRENT_BINARY_DIR}" PRIVATE "src/")
target_warnings_as_errors(TinyEngine-shared)
target_enable_clang_tidy(TinyEngine-shared)

if (LINUX)
	target_link_libraries(TinyEngine-static PUBLIC dl)
	target_link_libraries(TinyEngine-shared PUBLIC dl)
endif()

if (${TINY_ENGINE_BUILD_STATIC})
	add_library(TinyEngine::TinyEngine ALIAS TinyEngine-static)
else()
	add_library(TinyEngine::TinyEngine ALIAS TinyEngine-shared)
endif()

if (${TINY_ENGINE_BUILD_TESTS})
	add_subdirectory("test/")
endif()
