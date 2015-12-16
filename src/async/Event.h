#pragma once

#include <functional>
#include <vector>

#include "AsyncObject.h"
#include "EventQueue.h"

typedef std::function<void(void*)> EventCallback;

class Event;

class EventSubscription : public AsyncObject {
public:
	EventQueue *eventQueue = EventQueue::current;
	EventCallback callback;
	Event *event;

	EventSubscription(EventCallback callback, Event *event) : callback(callback), event(event) {

	}

	//TODO: Cancel

	virtual void handleMessage(void *data);
};

class Event {
private:
	std::vector<EventSubscription*> subscriptions;

public:
	//TODO: Cancel all on delete
	//TODO: Delete all on delete
	//TODO: Maybe add reference counting?
	EventSubscription *listen(EventCallback cb) {
		EventSubscription *subscription = new EventSubscription(cb, this);
		subscriptions.emplace_back(subscription);
		return subscription;
	}

	void fire(void *data) {
		for (auto sub : subscriptions) {
			sub->eventQueue->send(sub, data);
		}
	}
};
