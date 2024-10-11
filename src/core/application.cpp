#include "core/application.hpp"

namespace tiny_engine
{
	void Application::handleResize(uint32_t width, uint32_t height)
	{
		(void)(width);
		(void)(height);
	}

	char const* Application::name() const
	{
		return "A Tiny Engine Application";
	}
} // namespace tiny_engine
