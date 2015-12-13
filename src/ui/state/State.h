#pragma once

#include <gccore.h>

#include <thread/ui/UIMain.h>

#include <ui/element/UIElement.h>

#include <util/Mutex.h>

class State {
private:
	UIElementType uiType;
	UIElement *element = nullptr;

public:
	Mutex mutex;
	bool loaded = false;

	State(UIElementType type) : uiType(type) {};
	virtual ~State() {};

	void lock() {
		mutex.lock();
	}

	void unlock() {
		mutex.unlock();
	}

	//This is called in a Data Thread to load State data
	//There can be multiple data threads.
	//The mutex is already locked.
	//Flags can be used to load specific parts, for example
	//HomebrewApp does not load the description field on initial load
	//So HomebrewAppDialogState tells it to load
	virtual void load(u32 flags) = 0;

	//Called to unload bigger parts of data
	//Big data includes Textures
	//This is usually called by ListElement when a state is still loaded but is one screen away from the top or bottom
	//This method may be called before load, and this method may be called multiple times
	virtual void unload() = 0;

	friend UIElement *UI::getElement(State*);
	friend void UI::RemoveCommand::execute();
};
