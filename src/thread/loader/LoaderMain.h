#pragma once

#include <ui/state/State.h>

#include <util/Thread.h>
#include <util/MessageQueue.h>

#define NUM_LOADER_THREADS 2

namespace Loader {

extern MessageQueue loadQueue;
extern Thread *threads[NUM_LOADER_THREADS];

void start();
void stop();

struct LoaderMessage {
	State *state;
	u32 flags;

	LoaderMessage(State *state, u32 flags) :
		state(state), flags(flags) {}
};

inline void queue(State *state, u32 flags = 0) {
	loadQueue.send(new LoaderMessage(state, flags));
}

}
