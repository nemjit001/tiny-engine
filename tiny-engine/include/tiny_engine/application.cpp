#include "tiny_engine/application.hpp"

namespace tiny_engine
{
	AppVersion Application::version() const
	{
		return { 0, 0, 0 };
	}

	char const* Application::name() const
	{
		return "A Tiny Engine Application";
	}
} // namespace tiny_engine
