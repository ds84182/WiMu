#pragma once

#include <render/Renderable.h>

#include <thread/render/RenderMain.h>

//The UI thread manages an array of std::vectors for instances of UIElements that are detached
//When we want to use UI elements, instead of allocating and deallocating lots of memory
//We pull unused elements and reattach them to existing data
//Esentially saving memory...

//If you want to request the UI to update an element, call UI::update(State *state)
//If you want to add an element to the root UI, call UI::add(State *state)
//If you want to remove, call UI::remove(State *state)

//On the UI thread, if you are writing a custom element with child elements, call UI::getElement(State *state)
	//to get the element for the state

enum UIElementType {
	NULL_ELEMENT,
	HOMEBREW_APP_ELEMENT
};

class State;

class UIElement {
public:
	UIElementType type;

	UIElement(UIElementType t) : type(t) {};
	virtual ~UIElement() {};

	//Override these to handle when a State is attached or detached to your UI Element
	virtual void attach(State *s) = 0;
	virtual void detach() = 0;

	//Override this to update or change Renderables depending on State
	virtual Renderable *update() = 0;
};
