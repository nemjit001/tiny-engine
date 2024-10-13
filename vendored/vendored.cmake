###
# Vendored non cmake project setup
###

set(VENDORED_LIB_BASE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/vendored/")

# Add glfw subproject
set(GLFW_BUILD_DOCS OFF)
set(GLFW_BUILD_EXAMPLES OFF)
set(GLFW_BUILD_TESTS OFF)
add_subdirectory("${VENDORED_LIB_BASE_DIR}/glfw/")

# Add Volk subproject
add_subdirectory("${VENDORED_LIB_BASE_DIR}/volk/")

if (${TINY_ENGINE_BUILD_TESTS})
	# Add GoogleTest subproject
	set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
	add_subdirectory("${VENDORED_LIB_BASE_DIR}/googletest/")
endif()
