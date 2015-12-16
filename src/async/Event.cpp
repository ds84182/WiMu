#include "Event.h"

void EventSubscription::handleMessage(void *data) {
	callback(data);
}
