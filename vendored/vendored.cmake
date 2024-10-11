###
# Vendored non cmake project setup
###

set(VENDORED_LIB_BASE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/vendored/")

# Find Vulkan
find_package(Vulkan REQUIRED)

# Add glfw subproject
set(GLFW_BUILD_DOCS OFF)
set(GLFW_BUILD_EXAMPLES OFF)
set(GLFW_BUILD_TESTS OFF)
add_subdirectory("${VENDORED_LIB_BASE_DIR}/glfw/")

# Set up ImGui project
add_library(imgui STATIC "${VENDORED_LIB_BASE_DIR}/imgui/imgui.cpp" "${VENDORED_LIB_BASE_DIR}/imgui/imgui_demo.cpp"
	"${VENDORED_LIB_BASE_DIR}/imgui/imgui_draw.cpp" "${VENDORED_LIB_BASE_DIR}/imgui/imgui_tables.cpp"
	"${VENDORED_LIB_BASE_DIR}/imgui/imgui_widgets.cpp"
	"${VENDORED_LIB_BASE_DIR}/imgui/backends/imgui_impl_glfw.cpp"		# GLFW hook
	"${VENDORED_LIB_BASE_DIR}/imgui/backends/imgui_impl_vulkan.cpp"		# Vulkan hook
)
target_include_directories(imgui PUBLIC "${VENDORED_LIB_BASE_DIR}/imgui/")
target_link_libraries(imgui PUBLIC glfw Vulkan::Vulkan)

# Add VMA subproject
add_subdirectory("${VENDORED_LIB_BASE_DIR}/VMA/")
