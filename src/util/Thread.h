#pragma once

#include <ogc/lwp.h>

typedef void *(*ThreadEntryPoint)(void *);

class Thread {
	lwp_t thread;
	
	public:
	Thread(ThreadEntryPoint entry, void *arg = nullptr, int priority = 64) {
		LWP_CreateThread(&thread, entry, arg, nullptr, 0, priority);
		//TODO: Throw exception if result < 0
	}
	
	void *join() {
		void *returnValue;
		LWP_JoinThread(thread, &returnValue);
		return returnValue;
	}
	
	void suspend() {
		LWP_SuspendThread(thread);
	}
	
	void resume() {
		LWP_ResumeThread(thread);
	}
};
