#include "Future.h"

Completer::Completer() {
	future = new Future();
}

void Completer::complete(void *data) {
	Logger::logf("Complete %p %p", data, future->listeningQueue);
	if (future->listeningQueue) {
		future->listeningQueue->send(future, data);
	} else {
		delete future;
	}

	delete this;
}

void Future::handleMessage(void *data) {
	void *ret = nullptr;
	if (callback) {
		ret = callback(data);
	}

	if (completer) {
		completer->complete(ret);
	}

	delete this;
}
