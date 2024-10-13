#pragma once

#include <cstdint>

namespace tiny_engine
{
	class CommandlineArgs;

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

		Application(Application const&) = delete;
		Application& operator=(Application const&) = delete;

		virtual bool init(CommandlineArgs const& args) = 0;

		virtual void shutdown() = 0;

		virtual bool update() = 0;

		virtual void render() = 0;

		/// @brief This function is used to retrieve the application version by the engine.
		/// Override it to set the application version to a meaningful value.
		/// @return 
		virtual AppVersion version() const;

		/// @brief This function is used to retrieve the application name by the engine.
		/// Override it to change the application name.
		/// @return 
		virtual char const* name() const;
	};
} // namespace tiny_engine
