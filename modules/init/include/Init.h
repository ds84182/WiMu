#pragma once

#include <util/Module.h>

namespace ModuleInit {
	enum RPCMessageType {
		HIDE
	};
	
	struct RPCMessage {
		RPCMessageType type;
		/*union {
			
		}*/
		
		RPCMessage(RPCMessageType type) : type(type) {
		
		}
	};
	
	inline void hide() {
		Module *init = Module::get("init");
		
		if (init) {
			init->waitReady();
			
			MessageQueue *queue = reinterpret_cast<MessageQueue*>(init->data["rpc"]);
			queue->send(new RPCMessage(HIDE));
		}
	}
}
