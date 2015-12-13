#include "RenderableText.h"

#include <ogc/gu.h>
#include <ogc/gx.h>

#include "GlobalMatrix.h"

RenderableText::~RenderableText() {
	delete[] chars;
}

void RenderableText::render() {
	Mtx	modelView;
	
	guMtxIdentity(modelView);
	guMtxTransApply(modelView, modelView, x, y, 0.0F);
	guMtxConcat(viewMatrix, modelView, modelView);
	
	GX_LoadPosMtxImm(modelView,	GX_PNMTX0);
	
	GXColor col = reinterpret_cast<GXColor&>(color);
	
	font->drawText(0, 0, chars, col);
}
