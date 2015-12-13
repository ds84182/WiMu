#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <gccore.h>
#include <malloc.h>

#include "Renderable.h"
#include "Texture.h"

class RenderableTexture : public Renderable {
	public:
	f32 x, y, scaleX, scaleY;
	u32 *color;
	Texture *texture;
	
	RenderableTexture(Texture *tex, f32 x, f32 y, f32 sx = 1, f32 sy = 1) :
		x(x), y(y), scaleX(sx), scaleY(sy), texture(tex) {
		color = (u32*)memalign(32, 4);
		*color = 0xFFFFFFFF;
	};

	virtual ~RenderableTexture();
	virtual void render();
};
