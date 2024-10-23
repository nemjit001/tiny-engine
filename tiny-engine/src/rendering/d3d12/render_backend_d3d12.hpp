#pragma once

#include "tiny_engine/rendering/render_backend.hpp"

namespace tiny_engine::rendering
{
	class D3D12RenderBackend
		:
		public IRenderBackend
	{
	public:
		D3D12RenderBackend() = default;
		virtual ~D3D12RenderBackend() = default;

		virtual GraphicsDevice const* TINY_ENGINE_APICALL enumerateDevices(uint32_t& count) const override final;
	};
} // namespace tiny_engine::rendering
