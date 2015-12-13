#pragma once

#include <deque>

#include "Mutex.h"

template <class T> class ObjectPool {
Mutex mutex;
std::deque<T> available;

public:
	~ObjectPool() {
		for (T obj : available) {
			delete obj;
		}
	}
	
	T obtain() {
		T ret = nullptr;

		mutex.lock();
		if (!available.empty()) {
			ret = available.back();
			available.pop_back();
		}
		mutex.unlock();

		return ret;
	}

	void release(T obj) {
		mutex.lock();
		available.emplace_back(obj);
		mutex.unlock();
	}

	void clear() {
		mutex.lock();
		for (T obj : available) {
			delete obj;
		}
		available.clear();
		mutex.unlock();
	}
};
