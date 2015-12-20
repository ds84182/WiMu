#pragma once

#include <ogc/lwp.h>

class SyncQueue {
	lwpq_t queue;

public:
	SyncQueue() {
		LWP_InitQueue(&queue);
	}

	void sleep() {
		LWP_ThreadSleep(queue);
	}

	void signal() {
		LWP_ThreadSignal(queue);
	}

	void broadcast() {
		LWP_ThreadBroadcast(queue);
	}
};
