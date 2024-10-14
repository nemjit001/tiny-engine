#include "tiny_engine/application.hpp"

namespace tiny_engine
{
	bool Application::running = true;

	void Application::handleResize(uint32_t width, uint32_t height)
	{
		(void)(width);
		(void)(height);
	}

	void Application::exit()
	{
		running = false;
	}

	bool Application::isRunning() const
	{
		return running;
	}

	bool Application::allowWindowResize() const
	{
		return false;
	}

	AppVersion Application::version() const
	{
		return { 0, 0, 0 };
	}

	char const* Application::name() const
	{
		return "A Tiny Engine Application";
	}
} // namespace tiny_engine
