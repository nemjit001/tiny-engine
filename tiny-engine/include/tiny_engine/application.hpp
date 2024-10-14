#pragma once

#include <cstdint>

namespace tiny_engine
{
	class CommandlineArgs;
	class WindowSystem;

	/// @brief Semantic versioning info for Application.
	struct AppVersion
	{
		uint32_t major : 10;
		uint32_t minor : 10;
		uint32_t patch : 12;
	};

	/// @brief Application interface used by the `tiny_engine::Engine` class.
	/// Child classes should implement their required virtual functions, and use the `TINY_ENGINE_MAKE_ENTRYPOINT`
	/// macro to register their class with the engine runtime.
	class Application
	{
	public:
		Application() = default;
		virtual ~Application() = default;

		Application(Application const&) noexcept = default;
		Application& operator=(Application const&) noexcept = default;

		/// @brief Initialize the application post construction.
		/// @param args Commandline arguments passed to Engine.
		/// @param pWindowSystem window system of the Engine.
		/// @return A boolean indicating successful initialization.
		virtual bool init(CommandlineArgs const& args, WindowSystem* pWindowSystem) = 0;

		/// @brief Shut down the application befor destruction. Any non RAII members should be cleaned up here.
		virtual void shutdown() = 0;

		/// @brief Update the application state.
		/// @return A boolean indicating successful update.
		virtual bool update() = 0;

		/// @brief Render using the application.
		virtual void render() = 0;

		/// @brief This function is used to recieve application window resize events, it may be overridden to modify resize behaviour.
		/// @param width New window width.
		/// @param height New window height.
		virtual void handleResize(uint32_t width, uint32_t height);

		/// @brief Exit the application.
		virtual void exit();

		/// @brief Check if the application is still running (i.e. was not marked for exit).
		/// @return 
		virtual bool isRunning() const;

		/// @brief Mark application as supporting window resize. Default is false, override this function to set a custom value.
		/// @return 
		virtual bool allowWindowResize() const;

		/// @brief This function is used to retrieve the application version by the engine.
		/// Override it to set the application version to a meaningful value.
		/// @return 
		virtual AppVersion version() const;

		/// @brief This function is used to retrieve the application name by the engine.
		/// Override it to change the application name.
		/// @return 
		virtual char const* name() const;

	protected:
		static bool running;
	};
} // namespace tiny_engine
