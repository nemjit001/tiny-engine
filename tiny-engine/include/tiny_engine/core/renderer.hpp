#pragma once

namespace tiny_engine
{
	/// @brief This is the Renderer interface. This interface must be implemented by renderer plugins for specific graphics API support.
	class IRenderer
	{
	public:
		IRenderer() = default;
		virtual ~IRenderer() = default;

		IRenderer(IRenderer const&) = default;
		IRenderer& operator=(IRenderer const&) = default;
	};
} // namespace tiny_engine
