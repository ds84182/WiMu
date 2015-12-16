#include "EventQueue.h"

EventQueue *EventQueue::current = nullptr;
Mutex *EventQueue::eventMutex = nullptr;

static void *eventQueueThreadEntryPoint(void *data) {
	Logger::log("Event Queue Thread started");

	EventQueueThreadArgs *args = reinterpret_cast<EventQueueThreadArgs*>(data);
	ThreadEntryPoint entry = args->entry;
	void *entryArgs = args->args;

	EventQueue queue;

	queue.begin();
	entry(entryArgs);
	queue.end();

	while (!args->stopThread)
		queue.process();

	return nullptr;
}

EventQueueThread::EventQueueThread(ThreadEntryPoint entry, void *targs) {
	args = new EventQueueThreadArgs();
	args->entry = entry;
	args->args = targs;
	args->stopThread = 0;
	args->queue = nullptr;

	thread = new Thread(eventQueueThreadEntryPoint, args);
}
