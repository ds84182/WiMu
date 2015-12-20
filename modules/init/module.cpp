#include <util/Log.h>
#include <render/RenderableCircle.h>
#include <render/RenderableRectangle.h>
#include <render/RenderableText.h>
#include <thread/render/RenderMain.h>
#include <util/Animation.h>
#include <util/Module.h>
#include <util/MessageQueue.h>

#include <Init.h>

using namespace Module::Init;

extern "C" void *module(Module* module) {
	Logger::log("Init Module is running");
	
	RenderableCircle *circle = new RenderableCircle(0, 480, 0, 0x3f51b5ff);
	Renderer::addRenderable(circle);
	Renderer::runAnimation(new AnimatableFloat(&circle->size, 0, 640*1.41), 0.5f, EasingFunction::IN_QUAD);
	
	RenderableText *title = new RenderableText("WiMu", 40, 128, 0xFFFFFFFF, Roboto::Title);
	Renderer::addRenderable(title);

	/*Renderer::runAnimation(new AnimatableFloat(&toolbar->y, 480, 0), 0.5f, EasingFunction::OUT_QUAD);
	Renderer::runAnimation(new AnimatableColor(toolbar->color, 0x3f51b500, 0x3f51b5ff), 5, EasingFunction::OUT_QUAD);
	Renderer::runAnimation(new AnimatableFloat(&page->y, 480+128, 128), 0.7f, EasingFunction::OUT_QUAD);
	Renderer::runAnimation(new AnimatableFloat(&title->y, 480+128-4, 128-4), 0.5f, EasingFunction::OUT_QUAD);*/

	RenderableText *text = new RenderableText("Hello from init module", 40, 128+Roboto::BodySize+8, 0xFFFFFFFF);
	Renderer::addRenderable(text);
	
	Renderer::runAnimation(new AnimatableColor(&title->color, 0xFFFFFF00, 0xFFFFFFFF), 0.3f, EasingFunction::OUT_QUAD, 0.25f);
	Renderer::runAnimation(new AnimatableColor(&text->color, 0xFFFFFF00, 0xFFFFFFFF), 0.3f, EasingFunction::OUT_QUAD, 0.375f);
	
	MessageQueue rpcQueue;
	
	while (true) {
		RPCMessage *message = reinterpret_cast<RPCMessage*>(rpcQueue.receive());
		if (message->type == HIDE) {
			AnimatableColor *ani = new AnimatableColor(circle->color, 0x3f51b5ff, 0x3f51b500);
			Renderer::runAnimation(ani, 0.3f, EasingFunction::OUT_QUAD);
			Renderer::runAnimation(new AnimatableColor(&title->color, 0xFFFFFFFF, 0xFFFFFF00), 0.3f, EasingFunction::OUT_QUAD);
			Renderer::runAnimation(new AnimatableColor(&text->color, 0xFFFFFFFF, 0xFFFFFF00), 0.3f, EasingFunction::OUT_QUAD);
			
			Renderer::waitAnimation(ani);
			
			Renderer::removeRenderable(circle);
			Renderer::removeRenderable(title);
			Renderer::removeRenderable(text);
			
			Renderer::sync();
			
			delete circle;
			delete title;
			delete text;
		}
	}
	
	return nullptr;
}
