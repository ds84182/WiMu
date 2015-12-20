#pragma once

#include <render/Renderable.h>
#include <render/RenderMessage.h>

#include <util/Animation.h>
#include <util/Thread.h>
#include <util/MessageQueue.h>
#include <util/SyncQueue.h>

namespace Renderer {

extern MessageQueue renderQueue;
extern SyncQueue renderSync;
extern Thread *thread;

void start();
void stop();

inline void addRenderable(Renderable *renderable) {
	RenderMessage *addRenderable = new RenderMessage();
	addRenderable->type = ADD_RENDERABLE;
	addRenderable->renderable = renderable;
	renderQueue.send(addRenderable);
}

inline void addRenderableFirst(Renderable *renderable) {
	RenderMessage *addRenderable = new RenderMessage();
	addRenderable->type = ADD_RENDERABLE_FIRST;
	addRenderable->renderable = renderable;
	renderQueue.send(addRenderable);
}

inline void removeRenderable(Renderable *renderable) {
	RenderMessage *addRenderable = new RenderMessage();
	addRenderable->type = REMOVE_RENDERABLE;
	addRenderable->renderable = renderable;
	renderQueue.send(addRenderable);
}

inline void runAnimation(Animatable *ani, f32 duration, EasingFunc f, f32 delay = 0.0f) {
	RenderMessage *runAnimation = new RenderMessage();
	runAnimation->type = RUN_ANIMATION;
	runAnimation->runAnimationArgs = new RenderMessageAnimationArgs(ani, duration, f, delay);
	renderQueue.send(runAnimation);
}

inline void stopAnimation(Animatable *ani) {
	RenderMessage *stopAnimation = new RenderMessage();
	stopAnimation->type = STOP_ANIMATION;
	stopAnimation->animationSubject = ani;
	renderQueue.send(stopAnimation);
}

inline void sync() {
	renderSync.sleep();
}

void *main(void *args);

}
