#pragma once

#include <render/Renderable.h>
#include <render/RenderMessage.h>

#include <util/Thread.h>
#include <util/MessageQueue.h>

namespace Renderer {

extern MessageQueue renderQueue;
extern Thread *thread;

void start();
void stop();

inline void addRenderable(Renderable *renderable) {
	RenderMessage *addRenderable = new RenderMessage();
	addRenderable->type = ADD_RENDERABLE;
	addRenderable->renderable = renderable;
	renderQueue.send(addRenderable);
}

inline void removeRenderable(Renderable *renderable) {
	RenderMessage *addRenderable = new RenderMessage();
	addRenderable->type = REMOVE_RENDERABLE;
	addRenderable->renderable = renderable;
	renderQueue.send(addRenderable);
}

void *main(void *args);

}
