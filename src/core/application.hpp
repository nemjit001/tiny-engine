#pragma once

#include <cstdint>

namespace tiny_engine
{
	/// @brief Application interface used by the `tiny_engine::Engine` class.
	/// Child classes should implement their required virtual functions, and use the `TINY_ENGINE_MAKE_ENTRYPOINT`
	/// macro to register their class with the engine runtime.
	class Application
	{
	public:
		Application() = default;
		virtual ~Application() = default;

		Application(Application const&) = delete;
		Application& operator=(Application const&) = delete;

		virtual bool init() = 0;

		virtual void shutdown() = 0;

		virtual bool update() = 0;

		virtual void render() = 0;

		/// @brief Resize callback used by the engine to pass resize information the application. Override this to customize resize handling.
		/// @param width 
		/// @param height 
		virtual void handleResize(uint32_t width, uint32_t height);

		/// @brief This function is used to retrieve the application name in the engine.
		/// Override it to change the application name.
		/// @return 
		virtual char const* name() const;
	};
} // namespace tiny_engine
