#include <util/Log.h>
#include <render/RenderableCircle.h>
#include <render/RenderableRectangle.h>
#include <render/RenderableText.h>
#include <thread/render/RenderMain.h>
#include <util/Animation.h>
#include <util/Module.h>
#include <util/MessageQueue.h>

#include <unistd.h>

#include <Init.h>

extern "C" void *module(Module* module) {
	Logger::log("Hello Module is running");
	
	RenderableText *title = new RenderableText("Hello Module", 40, 128, 0xFFFFFFFF, Roboto::Title);
	Renderer::addRenderableFirst(title);

	RenderableText *text = new RenderableText("Hello, World!", 40, 128+Roboto::BodySize+8, 0xFFFFFFFF);
	Renderer::addRenderableFirst(text);
	
	Renderer::runAnimation(new AnimatableColor(&title->color, 0xFFFFFF00, 0xFFFFFFFF), 0.3f, EasingFunction::OUT_QUAD, 0.25f);
	Renderer::runAnimation(new AnimatableColor(&text->color, 0xFFFFFF00, 0xFFFFFFFF), 0.3f, EasingFunction::OUT_QUAD, 0.375f);
	
	module->ready();
	
	ModuleInit::hide();
	
	return nullptr;
}
