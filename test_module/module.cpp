#include <util/Log.h>
#include <render/RenderableRectangle.h>
#include <render/RenderableText.h>
#include <thread/render/RenderMain.h>
#include <util/Animation.h>

extern "C" void *module(void *args) {
	Logger::log("testing logger");

	RenderableRectangle *toolbar = new RenderableRectangle(0, 0, 640, 128, 0x3f51b5ff);
	Renderer::addRenderable(toolbar);

	RenderableRectangle *page = new RenderableRectangle(0, 128, 640, 480-128, 0xf9f9f9ff);
	Renderer::addRenderable(page);

	RenderableText *title = new RenderableText("WiMu", 40, 128-4, 0xFFFFFFFF, Roboto::Title);
	Renderer::addRenderable(title);

	Renderer::runAnimation(new AnimatableFloat(&toolbar->y, 480, 0), 0.5f, EasingFunction::OUT_QUAD);
	Renderer::runAnimation(new AnimatableColor(toolbar->color, 0x3f51b500, 0x3f51b5ff), 5, EasingFunction::OUT_QUAD);
	Renderer::runAnimation(new AnimatableFloat(&page->y, 480+128, 128), 0.7f, EasingFunction::OUT_QUAD);
	Renderer::runAnimation(new AnimatableFloat(&title->y, 480+128-4, 128-4), 0.5f, EasingFunction::OUT_QUAD);

	RenderableText *text = new RenderableText("Hello from module", 40, 256, 0x000000FF);
	Renderer::addRenderable(text);
}
