#include "core/application.hpp"
#include "core/engine.hpp"

#include <cstdio>

class TinyRenderer
	:
	public tiny_engine::Application
{
public:
	TinyRenderer() = default;
	virtual ~TinyRenderer() = default;

	virtual bool init() override final;

	virtual void shutdown() override final;

	virtual bool update() override final;

	virtual void render() override final;

	virtual void handleResize(uint32_t width, uint32_t height) override final;

	virtual char const* name() const override final;
};

bool TinyRenderer::init()
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
	(void)(width);
	(void)(height);
}

char const* TinyRenderer::name() const
{
	return "Tiny Renderer";
}

TINY_ENGINE_MAKE_ENTRYPOINT(TinyRenderer);
