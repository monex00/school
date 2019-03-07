#ifndef __SOCKET_TCP_HPP
#define __SOCKET_TCP_HPP

#include "../libs/Address.hpp"
#include "Connection.hpp"
#include <unistd.h>

class SocketTCP {
	protected: int socketID;
	public: SocketTCP();
			~SocketTCP();
};

SocketTCP::SocketTCP() {
	this->socketID = socket(AF_INET, SOCK_STREAM, 0);
}

SocketTCP::~SocketTCP() {
	close(this->socketID);
}

#endif //__SOCKET_TCP_HPP
