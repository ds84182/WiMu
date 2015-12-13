#include "RenderableGroup.h"

#include "GlobalMatrix.h"

RenderableGroup::~RenderableGroup() {
	for (Renderable *r : children) {
		delete r;
	}
}

void RenderableGroup::render() {
	mutex.lock();
	pushMatrix();
	guMtxTransApply(viewMatrix, viewMatrix, x, y, 0.0F);
	for (Renderable *r : children) {
		if (r) {
			r->render();
		}
	}
	popMatrix();
	mutex.unlock();
}
