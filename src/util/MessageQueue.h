#pragma once

#include <ogc/message.h>

class MessageQueue {
	mqbox_t queue;
	
	public:
	MessageQueue(u32 count = 64) {
		MQ_Init(&queue, count);
	}
	
	void close() {
		MQ_Close(queue);
		queue = MQ_BOX_NULL;
	}
	
	void send(mqmsg_t message, u32 flags = MQ_MSG_NOBLOCK) {
		MQ_Send(queue, message, flags);
	}
	
	mqmsg_t receive(u32 flags = MQ_MSG_BLOCK) {
		mqmsg_t ret = nullptr;
		MQ_Receive(queue, &ret, flags);
		return ret;
	}
};
