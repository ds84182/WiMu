#pragma once

#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "Network.h"
#include "Mutex.h"

namespace Logger {
	extern TCPSocket *socket;
	extern Mutex *lock;
	
	inline void log(const char *s);
	
	inline void init() {
		u32 addr = 0;
		Network::lookup("natsu.lan", addr);
		socket = new TCPSocket(addr, 1343);
		lock = new Mutex();
		Logger::log("Logging system connected");
	}
	
	inline void deinit() {
		if (!socket) return;
		
		socket->close();
	}
	
	inline void log(const char *s) {
		if (!socket) return;
		
		printf("%s\n", s);
		
		lock->lock();
		socket->send(s, strlen(s));
		socket->send("\n");
		lock->unlock();
	}
	
	inline void logf(const char* format, ...) {
		if (!socket) return;
		
		char dest[1024];
		va_list argptr;
		va_start(argptr, format);
		vsnprintf(dest, 1024, format, argptr);
		va_end(argptr);
		log(dest);
	}
};
