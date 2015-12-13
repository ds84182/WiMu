#pragma once

#include <ogc/gu.h>
#include <string.h>

#define MATRIX_STACK 64

extern Mtx viewMatrix;
extern Mtx viewMatrixStack[MATRIX_STACK];
extern int viewMatrixStackIndex;
extern Mtx44 projectionMatrix;

inline void resetMatrixStack() {
	viewMatrixStackIndex = 0;
}

inline void pushMatrix() {
	memcpy(viewMatrixStack[viewMatrixStackIndex++], viewMatrix, sizeof(Mtx));
}

inline void popMatrix() {
	memcpy(viewMatrix, viewMatrixStack[--viewMatrixStackIndex], sizeof(Mtx));
}
