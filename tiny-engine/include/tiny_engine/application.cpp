#include "tiny_engine/application.hpp"

#include "tiny_engine/defines.hpp"
#include "tiny_engine/core/window_system.hpp"

namespace tiny_engine
{
	bool IApplication::running = true;

	void IApplication::handleResize(WindowSize const& size)
	{
		TINY_ENGINE_MARK_UNUSED(size);
	}

	void IApplication::exit()
	{
		running = false;
	}

	bool IApplication::isRunning() const
	{
		return running;
	}

	bool IApplication::allowWindowResize() const
	{
		return false;
	}

	AppVersion IApplication::version() const
	{
		return { 0, 0, 0 };
	}

	char const* IApplication::name() const
	{
		return "A Tiny Engine Application";
	}
} // namespace tiny_engine
