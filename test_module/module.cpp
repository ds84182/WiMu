#include <util/Log.h>
#include <render/RenderableText.h>
#include <thread/render/RenderMain.h>

extern "C" void *module(void *args) {
	Logger::log("testing logger");

	RenderableText *text = new RenderableText("Hello from module", 40, 256, 0x000000FF);
	Renderer::addRenderable(text);
}
