#include "LoaderMain.h"

#include <util/Log.h>

namespace Loader {

vu8 stopThreads = 0;
MessageQueue loadQueue;
Thread *threads[NUM_LOADER_THREADS];

void *main(void *);

void start() {
	int i;
	for (i=0; i<NUM_LOADER_THREADS; i++) {
		threads[i] = new Thread(Loader::main);
	}
}

void stop() {
	stopThreads = 1;

	int i;
	queue(nullptr);
	for (i=0; i<NUM_LOADER_THREADS; i++) {
		queue(nullptr);
	}

	for (i=0; i<NUM_LOADER_THREADS; i++) {
		threads[i]->join();
	}
}

// Actual Thread Code starts here

static vu8 nextID = 0;

void *main(void *args) {
	u8 id = nextID++;
	Logger::logf("Loader Thread %d starting", id);

	while (!stopThreads) {
		LoaderMessage *message = reinterpret_cast<LoaderMessage*>(loadQueue.receive());

		if (message) {
			if (message->state) {
				message->state->lock();
				message->state->load(message->flags);
				message->state->unlock();
			}

			delete message;
		}
	}

	return nullptr;
}

}
