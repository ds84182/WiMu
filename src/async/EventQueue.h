#pragma once

#include <util/Mutex.h>
#include <util/MessageQueue.h>
#include <util/Log.h>

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

	void process() {
		EventQueueMessage *message = reinterpret_cast<EventQueueMessage*>(queue.receive());

		eventMutex->lock();
		current = this;

		Logger::logf("Process message %p", message);
		while (message) {
			message->object->handleMessage(message->data);
			delete message;
			message = reinterpret_cast<EventQueueMessage*>(queue.receive(MQ_MSG_NOBLOCK));
		}

		current = nullptr;
		eventMutex->unlock();
	}
};
