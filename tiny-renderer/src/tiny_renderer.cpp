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

	virtual bool init(tiny_engine::core::CommandlineArgs const& args, tiny_engine::core::WindowSystem* pWindowSystem) override final;

	virtual void shutdown() override final;

	virtual bool update() override final;

	virtual void render() override final;

	virtual void handleResize(tiny_engine::core::WindowSize const& size) override final;

	virtual bool allowWindowResize() const override final;

	virtual tiny_engine::AppVersion version() const override final;

	virtual char const* name() const override final;
};

bool TinyRenderer::init(tiny_engine::core::CommandlineArgs const& args, tiny_engine::core::WindowSystem* pWindowSystem)
{
	TINY_ENGINE_MARK_UNUSED(args);
	TINY_ENGINE_MARK_UNUSED(pWindowSystem);
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

void TinyRenderer::handleResize(tiny_engine::core::WindowSize const& size)
{
	printf("Resize event: (%u x %u)\n", size.width, size.height);
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
