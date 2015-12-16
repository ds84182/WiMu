#include "Log.h"

namespace Logger {
	TCPSocket *socket = nullptr;
	Mutex *lock = nullptr;
};

extern "C" void log(const char *s) {
	Logger::log(s);
}
