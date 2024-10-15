#pragma once

namespace tiny_engine::rendering
{
	class IRenderer
	{
	public:
		IRenderer();
		virtual ~IRenderer();

		IRenderer(IRenderer const&) noexcept = default;
		IRenderer& operator=(IRenderer const&) noexcept = default;
	};
} // namespace tiny_engine::rendering
