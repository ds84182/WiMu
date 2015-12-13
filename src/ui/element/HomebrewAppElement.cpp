#include "HomebrewAppElement.h"

void HomebrewAppElement::attach(State *state) {
	app = (HomebrewAppState*) state;
}

void HomebrewAppElement::detach() {
	app = nullptr;
	textureRenderable->texture = nullptr;
	textRenderable->setText("");
}

Renderable *HomebrewAppElement::update() {
	if (app) {
		textureRenderable->texture = app->icon;
		textRenderable->setText(app->name.c_str());
	}

	return mainState;
}
