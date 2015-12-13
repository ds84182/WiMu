#pragma once

#include <string>

#include <network.h>

namespace Network {
	inline int init() {
		/*char localip[16] = {0};
		char gateway[16] = {0};
		char netmask[16] = {0};
	
		return if_config (localip, netmask, gateway, TRUE);*/
		return net_init();
	}
	
	inline void deinit() {
		net_deinit();
	}
	
	inline bool lookup(std::string host, u32 &address) {
		struct hostent *ent = net_gethostbyname(host.c_str());
		
		if (!ent) return false;
		
		address = ((in_addr*)ent->h_addr_list[0])->s_addr;
		return true;
	}
};

class Socket {
	protected:
	s32 socket;
	
	public:
	virtual ssize_t send(const void *ptr, size_t size, int flags = MSG_DONTWAIT) = 0;
	virtual ssize_t recv(void *ptr, size_t size, int flags = 0) = 0;
};

class TCPSocket : public Socket {
	public:
	TCPSocket(u32 address, u16 port) {
		socket = net_socket(PF_INET, SOCK_STREAM, IPPROTO_IP);
		
		if (socket >= 0) {
			//u32 set = 1;
			//net_setsockopt(socket, IPPROTO_TCP, TCP_NODELAY, &set, sizeof(set));
			
			struct sockaddr_in sa;
			memset(&sa, 0, sizeof(sa));
			
			sa.sin_family = AF_INET;
			sa.sin_port = htons(port);
			sa.sin_addr.s_addr = address;
			sa.sin_len = sizeof(sa);
			
			s32 result = net_connect(socket, (struct sockaddr *) &sa, sizeof(sa));
			
			if (result < 0) {
				net_close(socket);
				socket = -1;
			}
		}
	}
	
	bool isConnected() {
		return socket >= 0;
	}
	
	void close() {
		if (!isConnected()) return;
		
		net_shutdown(socket, 2); //SHUT_RDWR
		net_close(socket);
	}
	
	ssize_t send(const void *ptr, size_t size, int flags = 0) {
		if (!isConnected()) return -1;
		
		return net_send(socket, ptr, size, flags);
	}
	
	ssize_t send(std::string s, int flags = 0) {
		return send(s.c_str(), s.length(), flags);
	}
	
	ssize_t recv(void *ptr, size_t size, int flags = 0) {
		if (!isConnected()) return -1;
		
		return net_recv(socket, ptr, size, flags);
	}
};
