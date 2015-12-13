#include "EventQueue.h"

EventQueue *EventQueue::current = nullptr;
Mutex *EventQueue::eventMutex = nullptr;
