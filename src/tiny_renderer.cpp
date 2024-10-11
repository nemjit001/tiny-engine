#include "engine_config.h"
#include "core/application.hpp"
#include "core/engine.hpp"

class TinyRenderer
	:
	public tiny_engine::Application
{
public:
	TinyRenderer() = default;
	virtual ~TinyRenderer() = default;

	virtual bool init(tiny_engine::CommandlineArgs const& args) override final;

	virtual void shutdown() override final;

	virtual bool update() override final;

	virtual void render() override final;

	virtual void handleResize(uint32_t width, uint32_t height) override final;

	virtual tiny_engine::AppVersion version() const;

	virtual char const* name() const override final;
};

bool TinyRenderer::init(tiny_engine::CommandlineArgs const& args)
{
	(void)(args);
	return true;
}

void TinyRenderer::shutdown()
{
	//
}

bool TinyRenderer::update()
{
	return true;
}

void TinyRenderer::render()
{
	//
}

void TinyRenderer::handleResize(uint32_t width, uint32_t height)
{
	(void)(width);
	(void)(height);
}

tiny_engine::AppVersion TinyRenderer::version() const
{
	// FIXME(nemjit001): actually use renderer app version instead of engine version
	return tiny_engine::AppVersion{ TINY_ENGINE_VERSION_MAJOR, TINY_ENGINE_VERSION_MINOR, TINY_ENGINE_VERSION_PATCH };
}

char const* TinyRenderer::name() const
{
	return "Tiny Renderer";
}

TINY_ENGINE_MAKE_ENTRYPOINT(TinyRenderer);
