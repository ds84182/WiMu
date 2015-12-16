#pragma once

#include <render/Renderable.h>
#include <render/RenderMessage.h>

#include <util/Animation.h>
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

inline void runAnimation(Animatable *ani, f32 duration, EasingFunc f) {
	RenderMessage *runAnimation = new RenderMessage();
	runAnimation->type = RUN_ANIMATION;
	runAnimation->runAnimationArgs = new RenderMessageAnimationArgs(ani, duration, f);
	renderQueue.send(runAnimation);
}

inline void stopAnimation(Animatable *ani) {
	RenderMessage *stopAnimation = new RenderMessage();
	stopAnimation->type = STOP_ANIMATION;
	stopAnimation->animationSubject = ani;
	renderQueue.send(stopAnimation);
}

void *main(void *args);

}
