#include "tiny_engine/application.hpp"

#include "tiny_engine/defines.hpp"
#include "tiny_engine/core/window_system.hpp"

namespace tiny_engine
{
	bool IApplication::running = true;

	void TINY_ENGINE_APICALL IApplication::handleResize(core::WindowSize const& size)
	{
		TINY_ENGINE_MARK_UNUSED(size);
	}

	void TINY_ENGINE_APICALL IApplication::exit()
	{
		running = false;
	}

	bool TINY_ENGINE_APICALL IApplication::isRunning() const
	{
		return running;
	}

	bool TINY_ENGINE_APICALL IApplication::allowWindowResize() const
	{
		return false;
	}

	AppVersion TINY_ENGINE_APICALL IApplication::version() const
	{
		return { 0, 0, 0 };
	}

	char const* TINY_ENGINE_APICALL IApplication::name() const
	{
		return "A Tiny Engine Application";
	}
} // namespace tiny_engine
