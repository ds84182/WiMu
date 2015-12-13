#include "GlobalMatrix.h"

Mtx viewMatrix;
Mtx viewMatrixStack[MATRIX_STACK];
int viewMatrixStackIndex = 0;
Mtx44 projectionMatrix;
