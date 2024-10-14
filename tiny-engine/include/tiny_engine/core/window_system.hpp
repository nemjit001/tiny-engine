#pragma once

#include <cstdint>

#include "tiny_engine/defines.hpp"

#if		TINY_ENGINE_PLATFORM_WINDOWS
	#include <windows.h> // Needed for HWND handle type
#endif

#if		TINY_ENGINE_VULKAN_AVAILABLE
	#define VK_NO_PROTOTYPES
	#include <vulkan/vulkan.h>
#endif

namespace tiny_engine
{
	/// @brief The WindowDesc structure is used for passing initial window parameters to the WindowSystem on init.
	struct WindowDesc
	{
		char const* pTitle;
		uint32_t width;
		uint32_t height;
		bool resizable;
	};

	/// @brief The WindowSystem manages the application window initialization and management.
	class WindowSystem
	{
	public:
		WindowSystem();
		~WindowSystem();

		WindowSystem(WindowSystem const&) = delete;
		WindowSystem& operator=(WindowSystem const&) = delete;

		/// @brief Initialize the WindowSystem, creating the main application window.
		/// @param window Window description for the main application window.
		/// @return A boolean indicating successful initialization.
		bool init(WindowDesc const& window);

		/// @brief Shut down the WindowSystem.
		void shutdown();

		/// @brief Update the window state.
		/// @return A boolean indicating successful update.
		bool update();

		/// @brief Set the application window size.
		/// @param width 
		/// @param height 
		void setWindowSize(uint32_t width, uint32_t height);

		/// @brief Check if the application window is minimized.
		/// @return 
		bool minimized() const;

#if		TINY_ENGINE_PLATFORM_WINDOWS
		/// @brief Retrieve the HWND associated with the application window.
		/// WARNING: This is a Windows specific function, this is not available on non-windows platforms.
		/// @return 
		HWND getHWND() const;
#endif

#if		TINY_ENGINE_VULKAN_AVAILABLE
		/// @brief Retrieve the required Vulkan instance extensions for window support.
		/// @param pExtensionCount Number of extensions in the returned array.
		/// @return An array of extension names.
		char const** requiredVulkanInstanceExtensions(uint32_t* pExtensionCount) const;

		/// @brief Create a Vulkan surface for the main application window.
		/// @param instance 
		/// @param pAllocator 
		/// @param pSurface 
		/// @return 
		VkResult createVulkanWindowSurface(VkInstance instance, VkAllocationCallbacks const* pAllocator, VkSurfaceKHR* pSurface);
#endif
	};
} // namespace tiny_engine
