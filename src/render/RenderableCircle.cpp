#include "RenderableCircle.h"

#include <gccore.h>
#include <math.h>

#include "GlobalMatrix.h"
#include "VertexDescriptions.h"

#define VERTICES 128

static bool initialized = false;
static f32 vertices[VERTICES*2] ATTRIBUTE_ALIGN(32);

RenderableCircle::~RenderableCircle() {
	free(color);
}

void RenderableCircle::render() {
	if (!initialized) {
		initialized = true;

		f32 increment = M_TWOPI/((f32)VERTICES);
		int i;
		for (i=0; i<VERTICES; i++) {
			f32 angle = increment*((f32)i);
			vertices[i*2] = cosf(angle);
			vertices[(i*2)+1] = sinf(angle);
		}
	}

	Mtx scaleMatrix;
	Mtx	modelView;
	
	guMtxScale(scaleMatrix, size, size, 1.0F);
	guMtxTrans(modelView, x, y, 0.0F);
	guMtxConcat(modelView, scaleMatrix, modelView);
	guMtxConcat(viewMatrix, modelView, modelView);
	
	GX_ClearVtxDesc();
	GX_SetVtxDescv(I8_POSITION_COLOR);
	GX_SetTevOp(GX_TEVSTAGE0, GX_PASSCLR);
	
	GX_SetArray(GX_VA_POS, vertices, 2*sizeof(f32));
	GX_SetArray(GX_VA_CLR0, color, sizeof(u32));
	
	GX_LoadPosMtxImm(modelView,	GX_PNMTX0);

	GX_Begin(GX_TRIANGLEFAN, VA_F32, VERTICES);

	int i;
	for (i=0; i<VERTICES; i++) {
		GX_Position1x8(i);
		GX_Color1x8(0);
	}
	
	GX_End();
}
