#pragma once
// smeas stream: 5:19
#include <stdlib.h>
#include <stdio.h>
#include <gccore.h>
#include <malloc.h>

#include "Renderable.h"

class RenderableRectangle : public Renderable {
	public:
	f32 x, y, width, height;
	u32 *color;
	
	RenderableRectangle(f32 x, f32 y, f32 w, f32 h, u32 col = 0xFFFFFFFF) :
		x(x), y(y), width(w), height(h) {
		color = (u32*)memalign(32, 4);
		*color = col;
		DCFlushRange(color, sizeof(u32));
	};

	virtual ~RenderableRectangle();
	virtual void render();
};
