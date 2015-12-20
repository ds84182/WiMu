#include "RenderableTexture.h"

#include <gccore.h>

#include "GlobalMatrix.h"
#include "VertexDescriptions.h"

static s16 vertices[] ATTRIBUTE_ALIGN(32) = {
	0, 0,
	1, 0,
	1, 1,
	0, 1
};

static f32 texcoords[] ATTRIBUTE_ALIGN(32) = {
	0, 0,
	1, 0,
	1, 1,
	0, 1
};

RenderableTexture::~RenderableTexture() {
	free(color);
}

void RenderableTexture::render() {
	if (!texture) return;
	
	Mtx scaleMatrix;
	Mtx	modelView;
	
	guMtxScale(scaleMatrix, texture->width*scaleX, texture->height*scaleY, 1.0F);
	guMtxTrans(modelView, x, y, 0.0F);
	guMtxConcat(scaleMatrix, modelView, modelView);
	guMtxConcat(viewMatrix, modelView, modelView);
	
	GX_ClearVtxDesc();
	GX_SetVtxDescv(I8_POSITION_COLOR_TEX);
	GX_SetTevOp(GX_TEVSTAGE0, GX_MODULATE);

	texture->use();
	
	GX_SetArray(GX_VA_POS, vertices, 2*sizeof(s16));
	GX_SetArray(GX_VA_CLR0, color, sizeof(u32));
	GX_SetArray(GX_VA_TEX0, texcoords, 2*sizeof(f32));
	
	GX_LoadPosMtxImm(modelView,	GX_PNMTX0);

	GX_Begin(GX_QUADS, VA_S16, 4);

	GX_Position1x8(0);
	GX_Color1x8(0);
	GX_TexCoord1x8(0);
	
	GX_Position1x8(1);
	GX_Color1x8(0);
	GX_TexCoord1x8(1);
	
	GX_Position1x8(2);
	GX_Color1x8(0);
	GX_TexCoord1x8(2);
	
	GX_Position1x8(3);
	GX_Color1x8(0);
	GX_TexCoord1x8(3);
	
	GX_End();
}
