#pragma once

class Renderable {
	public:
	virtual ~Renderable() {};
	virtual void render() = 0;
};
