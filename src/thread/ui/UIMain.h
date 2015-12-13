#pragma once

#include <ui/element/UIElement.h>

#include <util/Thread.h>
#include <util/MessageQueue.h>

class State;

namespace UI {

extern MessageQueue uiQueue;
extern Thread *thread;

void start();
void stop();
UIElement *getElement(State *state);

struct UICommand {
	virtual ~UICommand() {};
	virtual void execute() = 0;
};

struct AddCommand : public UICommand {
	State *state;

	AddCommand(State *state) : state(state) {};
	virtual void execute();
};

struct RemoveCommand : public UICommand {
	State *state;

	RemoveCommand(State *state) : state(state) {};
	virtual void execute();
};

struct UpdateCommand : public UICommand {
	State *state;

	UpdateCommand(State *state) : state(state) {};
	virtual void execute();
};

//TODO: util/ObjectPool.h

inline void add(State *state) {
	uiQueue.send(new AddCommand(state));
}

inline void remove(State *state) {
	uiQueue.send(new RemoveCommand(state));
}

inline void update(State *state) {
	uiQueue.send(new UpdateCommand(state));
}

}
