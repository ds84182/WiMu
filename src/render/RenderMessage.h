#pragma once

#include "Renderable.h"

#include <util/Animation.h>

enum RenderMessageType {
	ADD_RENDERABLE,
	REMOVE_RENDERABLE,
	RUN_ANIMATION,
	STOP_ANIMATION
};

struct RenderMessageAnimationArgs {
	Animatable *subject;
	f32 duration;
	EasingFunc easing;

	RenderMessageAnimationArgs(Animatable *s, f32 dur, EasingFunc f) :
		subject(s), duration(dur), easing(f) {};
};

struct RenderMessage {
	RenderMessageType type;
	union {
		Renderable *renderable;
		RenderMessageAnimationArgs *runAnimationArgs;
		Animatable *animationSubject;
	};
};
