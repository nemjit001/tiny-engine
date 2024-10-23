#include <tiny_engine/application.hpp>
#include <tiny_engine/defines.hpp>
#include <tiny_engine/engine.hpp>

#include "app_config.h"

class TinyRenderer
	:
	public tiny_engine::IApplication
{
public:
	TinyRenderer() = default;
	virtual ~TinyRenderer() = default;

	virtual bool TINY_ENGINE_APICALL init(tiny_engine::core::CommandlineArgs const& args, tiny_engine::core::WindowSystem* pWindowSystem) override final;

	virtual void TINY_ENGINE_APICALL shutdown() override final;

	virtual bool TINY_ENGINE_APICALL update() override final;

	virtual void TINY_ENGINE_APICALL render() override final;

	virtual void TINY_ENGINE_APICALL handleResize(tiny_engine::core::WindowSize const& size) override final;

	virtual bool TINY_ENGINE_APICALL allowWindowResize() const override final;

	virtual tiny_engine::AppVersion TINY_ENGINE_APICALL version() const override final;

	virtual char const* TINY_ENGINE_APICALL name() const override final;
};

bool TINY_ENGINE_APICALL TinyRenderer::init(tiny_engine::core::CommandlineArgs const& args, tiny_engine::core::WindowSystem* pWindowSystem)
{
	TINY_ENGINE_MARK_UNUSED(args);
	TINY_ENGINE_MARK_UNUSED(pWindowSystem);
	return true;
}

void TINY_ENGINE_APICALL TinyRenderer::shutdown()
{
	//
}

bool TINY_ENGINE_APICALL TinyRenderer::update()
{
	return true;
}

void TINY_ENGINE_APICALL TinyRenderer::render()
{
	//
}

void TINY_ENGINE_APICALL TinyRenderer::handleResize(tiny_engine::core::WindowSize const& size)
{
	tiny_engine::core::Logging::Log(tiny_engine::core::LogLevel::Trace, "Resize event: (%u x %u)\n", size.width, size.height);
}

bool TINY_ENGINE_APICALL TinyRenderer::allowWindowResize() const
{
	return true;
}

tiny_engine::AppVersion TINY_ENGINE_APICALL TinyRenderer::version() const
{
	return tiny_engine::AppVersion{
		TINY_RENDERER_VERSION_MAJOR,
		TINY_RENDERER_VERSION_MINOR,
		TINY_RENDERER_VERSION_PATCH,
	};
}

char const* TINY_ENGINE_APICALL TinyRenderer::name() const
{
	return TINY_RENDERER_NAME;
}

TINY_ENGINE_MAKE_ENTRYPOINT(TinyRenderer);
