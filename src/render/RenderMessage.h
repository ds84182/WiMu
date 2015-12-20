#pragma once

#include "Renderable.h"

#include <util/Animation.h>

enum RenderMessageType {
	ADD_RENDERABLE,
	ADD_RENDERABLE_FIRST,
	REMOVE_RENDERABLE,
	RUN_ANIMATION,
	STOP_ANIMATION
};

struct RenderMessageAnimationArgs {
	Animatable *subject;
	f32 duration;
	EasingFunc easing;
	f32 delay;

	RenderMessageAnimationArgs(Animatable *s, f32 dur, EasingFunc f, f32 delay) :
		subject(s), duration(dur), easing(f), delay(delay) {};
};

struct RenderMessage {
	RenderMessageType type;
	union {
		Renderable *renderable;
		RenderMessageAnimationArgs *runAnimationArgs;
		Animatable *animationSubject;
	};
};
