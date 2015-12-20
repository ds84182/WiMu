#pragma once
// smeas stream: 5:19
#include <stdlib.h>
#include <stdio.h>
#include <gccore.h>
#include <malloc.h>

#include "Renderable.h"

class RenderableCircle : public Renderable {
	public:
	f32 x, y, size;
	u32 *color;
	
	RenderableCircle(f32 x, f32 y, f32 s, u32 col = 0xFFFFFFFF) :
		x(x), y(y), size(s) {
		color = (u32*)memalign(32, 4);
		*color = col;
		DCFlushRange(color, sizeof(u32));
	};

	virtual ~RenderableCircle();
	virtual void render();
};
