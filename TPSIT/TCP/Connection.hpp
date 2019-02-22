#ifndef __CONNECTION_HPP
#define __CONNECTION_HPP

#include "../libs/Address.hpp"
#include <unistd.h>

#define MAX_MSG_SIZE 4086

class Connection {
	protected: int connectionID;
			   Address connectedAddress;
	public: Connection(int, Address);
			bool send(char*);
			char* recv();
};

Connection::Connection(int connectionID, Address connectedAddress) {
	this->connectionID = connectionID;
	this->connectedAddress = connectedAddress;
}

bool Connection::send(char* msg) {
	ssize_t sendRet = ::send(this->connectionID, msg, strlen(msg) + 1, 0);
	return (sendRet != strlen(msg) + 1);
}

char* Connection::recv() {
	char buffer[MAX_MSG_SIZE + 1];
	ssize_t recvRet = ::recv(this->connectionID, buffer, MAX_MSG_SIZE, 0);
	return (recvRet == 0) ? NULL : strdup(buffer);
}

#endif //__CONNECTION_HPP
