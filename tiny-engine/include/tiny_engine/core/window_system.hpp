#pragma once

#include <cstdint>
#include <functional>

#include "tiny_engine/defines.hpp"

#if		TINY_ENGINE_PLATFORM_WINDOWS
	#include <windows.h>
#endif

#if		TINY_ENGINE_VULKAN_AVAILABLE
	#include <vulkan/vulkan.h>
#endif

namespace tiny_engine::core
{
	/// @brief The WindowDesc structure is used for passing initial window parameters to the WindowSystem on init.
	struct WindowDesc
	{
		char const* pTitle;
		uint32_t width;
		uint32_t height;
		bool resizable;
	};

	struct WindowSize
	{
		uint32_t width;
		uint32_t height;
	};

	/// @brief The WindowSystem manages the application window initialization and management.
	class TINY_ENGINE_API WindowSystem
	{
	public:
		using ResizeHandler = std::function<void(WindowSize const&)>;
		using CloseHandler = std::function<void()>;

		WindowSystem();
		~WindowSystem();

		WindowSystem(WindowSystem const&) = delete;
		WindowSystem& operator=(WindowSystem const&) = delete;

		/// @brief Initialize the WindowSystem, creating the main application window.
		/// @param window Window description for the main application window.
		/// @return A boolean indicating successful initialization.
		bool TINY_ENGINE_APICALL init(WindowDesc const& window);

		/// @brief Shut down the WindowSystem.
		void TINY_ENGINE_APICALL shutdown();

		/// @brief Update the window state.
		/// @return A boolean indicating successful update.
		bool TINY_ENGINE_APICALL update();

		/// @brief Set the application window size.
		/// @param size
		void TINY_ENGINE_APICALL setWindowSize(WindowSize const& size);

		/// @brief Get the current application window size.
		/// @return 
		WindowSize TINY_ENGINE_APICALL getWindowSize() const;

		/// @brief Set the window resize handler.
		/// @param handler 
		void TINY_ENGINE_APICALL setResizeHandler(ResizeHandler const& handler);

		/// @brief Set the application window close handler.
		/// @param handler 
		void TINY_ENGINE_APICALL setCloseHandler(CloseHandler const& handler);

		/// @brief Check if the application window is minimized.
		/// @return 
		bool TINY_ENGINE_APICALL minimized() const;

#if		TINY_ENGINE_PLATFORM_WINDOWS
		/// @brief Retrieve the HWND associated with the application window.
		/// WARNING: This is a Windows specific function, this is not available on non-windows platforms.
		/// @return 
		HWND TINY_ENGINE_APICALL getHWND() const;
#endif

#if		TINY_ENGINE_VULKAN_AVAILABLE
		/// @brief Retrieve the required Vulkan instance extensions for window support.
		/// @param pExtensionCount Number of extensions in the returned array.
		/// @return An array of extension names.
		char const** TINY_ENGINE_APICALL requiredVulkanInstanceExtensions(uint32_t* pExtensionCount) const;

		/// @brief Create a Vulkan surface for the main application window.
		/// @param instance 
		/// @param pAllocator 
		/// @param pSurface 
		/// @return 
		VkResult TINY_ENGINE_APICALL createVulkanWindowSurface(VkInstance instance, VkAllocationCallbacks const* pAllocator, VkSurfaceKHR* pSurface);
#endif
	};
} // namespace tiny_engine::core
