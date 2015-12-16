#pragma once

#include <functional>

#include "AsyncObject.h"
#include "EventQueue.h"

//typedef void *(*FutureCallback)(void *);
typedef std::function<void*(void*)> FutureCallback;

class Future;

class Completer : public AsyncObject {
public:
	Future *future;

	Completer();
	virtual ~Completer() {}

	void complete(void *data);
};

class Future : public AsyncObject {
private:
	EventQueue *listeningQueue = nullptr;
	FutureCallback callback = nullptr;
	Completer *completer = nullptr;
	friend void Completer::complete(void*);

public:
	virtual ~Future() {}

	Future *then(FutureCallback cb) {
		listeningQueue = EventQueue::current;
		callback = cb;
		completer = new Completer();

		return completer->future;
	}

	virtual void handleMessage(void *data);
};
