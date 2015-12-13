#include "RenderableTriangle.h"

#include <gccore.h>

#include "GlobalMatrix.h"
#include "VertexDescriptions.h"

static s16 vertices[] ATTRIBUTE_ALIGN(32) = {
	0, 128, 0,
	-128, -128, 0,
	128,	-128, 0};

static u8 colors[] ATTRIBUTE_ALIGN(32)	= {
	255, 0,	0, 255,		// red
	0, 255,	0, 255,		// green
	0, 0, 255, 255};	// blue

RenderableTriangle::~RenderableTriangle() {

}

void RenderableTriangle::render() {
	Mtx	modelView;
	
	guMtxIdentity(modelView);
	guMtxTransApply(modelView, modelView, 0.0F, 0.0F, 0.0F);
	guMtxConcat(viewMatrix, modelView, modelView);
	
	GX_ClearVtxDesc();
	GX_SetVtxDescv(I8_POSITION_COLOR);
	GX_SetTevOp(GX_TEVSTAGE0, GX_PASSCLR);
	
	GX_SetArray(GX_VA_POS, vertices, 3*sizeof(s16));
	GX_SetArray(GX_VA_CLR0,	colors,	4*sizeof(u8));
	
	GX_LoadPosMtxImm(modelView,	GX_PNMTX0);

	GX_Begin(GX_TRIANGLES, GX_VTXFMT0, 3);

	GX_Position1x8(0);
	GX_Color1x8(0);
	GX_Position1x8(1);
	GX_Color1x8(1);
	GX_Position1x8(2);
	GX_Color1x8(2);
	
	GX_End();
}
