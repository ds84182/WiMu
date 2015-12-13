#include "HomebrewAppElement.h"

void HomebrewAppElement::attach(State *state) {
	HomebrewAppState *app = (HomebrewAppState*) state;
	textureRenderable->texture = app->icon;
	textRenderable->setText(app->name.c_str());
}

void HomebrewAppElement::detach() {
	textureRenderable->texture = nullptr;
	textRenderable->setText("");
}

Renderable *HomebrewAppElement::update() {
	return mainState;
}
