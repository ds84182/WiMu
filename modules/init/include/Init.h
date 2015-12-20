#pragma once

#include <util/Module.h>

namespace Module {
	namespace Init {
		enum RPCMessageType {
			HIDE
		}
		
		struct RPCMessage {
			RPCMessageType type;
			/*union {
				
			}*/
			
			RPCMessage(RPCMessageType type) : type(type) {
			
			}
		}
		
		inline void hide() {
			Module *init = Module::get("init");
			
			if (init) {
				MessageQueue *queue = reinterpret_cast<MessageQueue*>(init->data["rpc"]);
				queue->send(new RPCMessage(HIDE));
			}
		}
	}
}
