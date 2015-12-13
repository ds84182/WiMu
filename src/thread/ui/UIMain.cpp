#include "UIMain.h"

#include <vector>
#include <map>

#include <ui/element/HomebrewAppElement.h>
#include <ui/state/State.h>

#include <util/Log.h>
#include <util/ObjectPool.h>

namespace UI {

vu8 stopThread = 0;
MessageQueue uiQueue;
Thread *thread;

void *main(void *);

void start() {
	thread = new Thread(UI::main);
}

void stop() {
	stopThread = 1;
	remove(nullptr);
	thread->join();
}

// Actual Thread Code starts here

struct UIElementMaker {
	virtual UIElement *make() = 0;
};

struct HomebrewAppElementMaker : public UIElementMaker {
	virtual UIElement *make() {
		return new HomebrewAppElement();
	}
};

ObjectPool<UIElement*> elementPools[UIElementType::MAX_ELEMENTS];
UIElementMaker* elementMakers[] = {
	nullptr,
	new HomebrewAppElementMaker(),
	nullptr
};
std::vector<State*> states;
std::map<State*, Renderable*> stateToRenderable;

UIElement *getElement(State *state) {
	if (state->element) {
		return state->element;
	} else {
		UIElement *element = elementPools[state->uiType].obtain();
		if (!element)
			element = elementMakers[state->uiType]->make();
		state->element = element;
		element->attach(state);
		return element;
	}
}

void addForRender(State *state) {
	UIElement *element = getElement(state);
	Renderable *r = element->update();
	Renderable *old = stateToRenderable[state];
	if (r != old) {
		if (old)
			Renderer::removeRenderable(old);

		stateToRenderable[state] = r;
		if (r)
			Renderer::addRenderable(r);
	}
}

void AddCommand::execute() {
	states.emplace_back(state);
	addForRender(state);
}

void RemoveCommand::execute() {
	UIElement *element = getElement(state);
	element->detach();
	state->element = nullptr;
	elementPools[element->type].release(element);

	states.erase(std::remove(states.begin(), states.end(), state));
	Renderable *r = stateToRenderable[state];
	if (r)
		Renderer::removeRenderable(r);
	stateToRenderable.erase(state);
}

void UpdateCommand::execute() {
	addForRender(state);
}

void *main(void *args) {
	Logger::log("UI Thread starting");

	while (!stopThread) {
		UICommand *command = reinterpret_cast<UICommand*>(uiQueue.receive());

		if (command) {
			command->execute();

			delete command;
		}
	}

	return nullptr;
}

}
