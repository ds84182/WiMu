#pragma once

#include <ogc/mutex.h>

class Mutex {
	mutex_t mutex;
	
	public:
	Mutex(bool recursive = false) {
		LWP_MutexInit(&mutex, recursive);
	}
	
	void lock() {
		LWP_MutexLock(mutex);
	}
	
	bool tryLock() {
		return LWP_MutexTryLock(mutex);
	}
	
	void unlock() {
		LWP_MutexUnlock(mutex);
	}
};
