#pragma once

#include "Renderable.h"

enum RenderMessageType {
	ADD_RENDERABLE,
	REMOVE_RENDERABLE
};

struct RenderMessage {
	RenderMessageType type;
	union {
		Renderable *renderable;
	};
};
