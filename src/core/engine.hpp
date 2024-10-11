#pragma once

namespace tiny_engine
{
	class Application;

	class Engine
	{
	public:
		Engine();
		~Engine();

		Engine(Engine const&) = delete;
		Engine& operator=(Engine const&) = delete;

		int run(Application* pApplication);
	};
} // namespace tiny_engine

#define TINY_ENGINE_MAKE_ENTRYPOINT(AppClass)	\
int main(int, char const**)						\
{												\
	AppClass app;								\
	return tiny_engine::Engine().run(&app);		\
}
