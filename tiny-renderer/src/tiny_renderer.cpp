#include <tiny_engine/application.hpp>
#include <tiny_engine/defines.hpp>
#include <tiny_engine/engine.hpp>

#include "app_config.h"

class TinyRenderer
	:
	public tiny_engine::Application
{
public:
	TinyRenderer() = default;
	virtual ~TinyRenderer() = default;

	virtual bool init(tiny_engine::CommandlineArgs const& args, tiny_engine::WindowSystem* pWindowSystem) override final;

	virtual void shutdown() override final;

	virtual bool update() override final;

	virtual void render() override final;

	virtual void handleResize(uint32_t width, uint32_t height) override final;

	virtual bool allowWindowResize() const override final;

	virtual tiny_engine::AppVersion version() const override final;

	virtual char const* name() const override final;
};

bool TinyRenderer::init(TINY_ENGINE_UNUSED tiny_engine::CommandlineArgs const& args, TINY_ENGINE_UNUSED tiny_engine::WindowSystem* pWindowSystem)
{
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
	printf("Resize event: (%u x %u)\n", width, height);
}

bool TinyRenderer::allowWindowResize() const
{
	return true;
}

tiny_engine::AppVersion TinyRenderer::version() const
{
	return tiny_engine::AppVersion{
		TINY_RENDERER_VERSION_MAJOR,
		TINY_RENDERER_VERSION_MINOR,
		TINY_RENDERER_VERSION_PATCH,
	};
}

char const* TinyRenderer::name() const
{
	return TINY_RENDERER_NAME;
}

TINY_ENGINE_MAKE_ENTRYPOINT(TinyRenderer);
