#pragma once

#include <algorithm>
#include <vector>

#include <util/Mutex.h>

#include "Renderable.h"

class RenderableGroup : public Renderable {
	private:
	Mutex mutex;
	std::vector<Renderable*> children;

	public:
	f32 x, y; //TODO: Maybe locking getters and setters?

	RenderableGroup(f32 x = 0, f32 y = 0) : x(x), y(y) {}
	RenderableGroup(std::vector<Renderable*> c, f32 x = 0, f32 y = 0) : children(c), x(x), y(y) {}
	RenderableGroup(Renderable** c, f32 x = 0, f32 y = 0) : x(x), y(y) {
		while (*c) {
			children.emplace_back(*c);
			c++;
		}
	}

	void addChild(Renderable* child) {
		mutex.lock();
		children.emplace_back(child);
		mutex.unlock();
	}

	void removeChild(Renderable* child) {
		mutex.lock();
		children.erase(std::remove(children.begin(), children.end(), child));
		mutex.unlock();
	}

	virtual ~RenderableGroup();
	virtual void render();
};
