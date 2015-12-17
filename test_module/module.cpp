#include <util/Log.h>

extern "C" void *module(void *args) {
	Logger::log("testing logger");
}
