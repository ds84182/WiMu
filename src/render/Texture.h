#pragma once

#include <malloc.h>
#include <ogc/gx.h>
#include <ogc/cache.h>
#include <pngu.h>
#include <string.h>

class Texture {
	public:
	void *data;
	u16 width, height;
	u8 fmt;
	bool freeableData = false;
	
	Texture(void *data, u16 width, u16 height, u8 fmt=GX_TF_RGBA8) :
		data(data), width(width), height(height), fmt(fmt) {};
	
	~Texture() {
		if (freeableData) {
			free(data);
		}
	}
	
	void use(u8 slot=GX_TEXMAP0) {
		GXTexObj object;
		GX_InitTexObj(&object, data, width, height, fmt, GX_CLAMP, GX_CLAMP, GX_FALSE);
		GX_LoadTexObj(&object, slot);
	}
	
	static Texture *createFromPNG(const unsigned char *png) {
		s32 width, height;
		
		PNGUPROP imgProp;
		IMGCTX ctx;
		
		ctx = PNGU_SelectImageFromBuffer(png);
		PNGU_GetImageProperties(ctx, &imgProp);
		void *data = PNGU_DecodeTo4x4RGBA8(ctx, imgProp.imgWidth, imgProp.imgHeight, &width, &height, nullptr);
		if (data) {
			if(imgProp.imgWidth != (u32)width || imgProp.imgHeight != (u32)height) {
				// PNGU has resized the texture
				memset(data, 0, (height * width) << 2);
			}
			DCFlushRange(data, width * height * 4);
			PNGU_ReleaseImageContext(ctx);
		
			Texture *tex = new Texture(data, width, height);
			tex->freeableData = true;
			return tex;
		}
		PNGU_ReleaseImageContext(ctx);
		
		return nullptr;
	}
};
