#pragma once

#include <volk.h>

namespace tiny_engine::core
{
	class Renderer
	{
	public:
		Renderer();
		~Renderer();

		Renderer(Renderer const&) = delete;
		Renderer& operator=(Renderer const&) = delete;
	};
} // namespace tiny_engine::core
