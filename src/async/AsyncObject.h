#pragma once

//Async Objects use message passing in order to do things
//Messages are sent to EventQueues, which will process
//those messages
class AsyncObject {
public:
	//TODO: Is this needed?
	/*Mutex mutex;

	void lock() {
		mutex.lock();
	}

	void unlock() {
		mutex.unlock();
	}*/

	virtual ~AsyncObject() {};
	virtual void handleMessage(void *data) {};
};
