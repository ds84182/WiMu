#pragma once

#include <util/Log.h>
#include <util/Mutex.h>
#include <util/MessageQueue.h>
#include <util/Thread.h>

#include "AsyncObject.h"

//TODO: Prefab EventQueueThread
//TODO: .begin() and .end() to call something in the
	//event queue context
//TODO: Stream AsyncObject

struct EventQueueMessage {
	AsyncObject *object;
	void *data;

	EventQueueMessage(AsyncObject *o, void* d) :
		object(o), data(d) {};
};

class EventQueue {
public:
	static EventQueue *current;
	static Mutex *eventMutex;

	MessageQueue queue;

	EventQueue() {
		if (!eventMutex)
			eventMutex = new Mutex();
	}

	void send(AsyncObject *object, void *data) {
		queue.send(new EventQueueMessage(object, data));
	}

	void begin() {
		eventMutex->lock();
		current = this;
	}

	void end() {
		current = nullptr;
		eventMutex->unlock();
	}

	void process() {
		EventQueueMessage *message = reinterpret_cast<EventQueueMessage*>(queue.receive());

		begin();

		Logger::logf("Process message %p", message);
		while (message) {
			message->object->handleMessage(message->data);
			delete message;
			message = reinterpret_cast<EventQueueMessage*>(queue.receive(MQ_MSG_NOBLOCK));
		}

		end();
	}
};

struct EventQueueThreadArgs {
	ThreadEntryPoint entry;
	void *args;
	vu8 stopThread;
	EventQueue *queue;
};

//OTOMATA, awwyis

class EventQueueThread {
private:
	EventQueueThreadArgs *args;
	Thread *thread;

public:
	EventQueueThread(ThreadEntryPoint entry, void *args = nullptr);

	void stop() {
		args->stopThread = 1;
		args->queue->queue.send(nullptr);
		thread->join();
		delete args;
		delete thread;
	}
};
