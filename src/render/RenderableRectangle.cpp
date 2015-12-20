#include "RenderableRectangle.h"

#include <gccore.h>

#include "GlobalMatrix.h"
#include "VertexDescriptions.h"

static s16 vertices[] ATTRIBUTE_ALIGN(32) = {
	0, 0,
	1, 0,
	1, 1,
	0, 1
};

RenderableRectangle::~RenderableRectangle() {
	free(color);
}

void RenderableRectangle::render() {
	Mtx scaleMatrix;
	Mtx	modelView;
	
	guMtxScale(scaleMatrix, width, height, 1.0F);
	guMtxTrans(modelView, x, y, 0.0F);
	guMtxConcat(modelView, scaleMatrix, modelView);
	guMtxConcat(viewMatrix, modelView, modelView);
	
	GX_ClearVtxDesc();
	GX_SetVtxDescv(I8_POSITION_COLOR);
	GX_SetTevOp(GX_TEVSTAGE0, GX_PASSCLR);
	
	GX_SetArray(GX_VA_POS, vertices, 2*sizeof(s16));
	GX_SetArray(GX_VA_CLR0, color, sizeof(u32));
	
	GX_LoadPosMtxImm(modelView,	GX_PNMTX0);

	GX_Begin(GX_QUADS, VA_S16, 4);

	GX_Position1x8(0);
	GX_Color1x8(0);
	GX_Position1x8(1);
	GX_Color1x8(0);
	GX_Position1x8(2);
	GX_Color1x8(0);
	GX_Position1x8(3);
	GX_Color1x8(0);
	
	GX_End();
}
