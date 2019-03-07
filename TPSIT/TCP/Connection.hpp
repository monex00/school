#ifndef __CONNECTION_HPP
#define __CONNECTION_HPP

#include "../libs/Address.hpp"
#include <unistd.h>
#include <fcntl.h>


#define MAX_MSG_SIZE 4086
#define MAX_FILE_CHUNK 512

class Connection {
	protected: int connectionID;
			   Address connectedAddress;
	public: Connection(int, Address);
			bool sendFile(char*);
			bool sendRaw(void*, int);
			bool send(char*);
			void* recvRaw(int*);
			char* recv();
};

Connection::Connection(int connectionID, Address connectedAddress) {
	this->connectionID = connectionID;
	this->connectedAddress = connectedAddress;
}

bool Connection::sendFile(char* path) {
	char buffer[MAX_FILE_CHUNK];
	
	int fd = open(path, O_RDONLY);
	if(fd == -1) return false;
	
	ssize_t nBytes = read(fd, buffer, MAX_FILE_CHUNK);
	while(nBytes && nBytes != -1) {
		this->sendRaw(buffer, nBytes);
		nBytes = read(fd, buffer, MAX_FILE_CHUNK);
	}
	
	close(fd);
	return (nBytes == 0) ? true : false;
}

bool Connection::sendRaw(void* msg, int len) {
	ssize_t sendRet = ::send(this->connectionID, msg, len, 0);
	return(sendRet != len);
}

bool Connection::send(char* msg) {
	return this->sendRaw((void*) msg, strlen(msg) + 1);
}

void* Connection::recvRaw(int* len) {
	char buffer[MAX_MSG_SIZE + 1];
	ssize_t recvRet = ::recv(this->connectionID, buffer, MAX_MSG_SIZE, 0);
	if (recvRet == 0)
		return NULL;
	
	*len = (int)recvRet;
	return strdup(buffer);
}

char* Connection::recv() {
	int len;
	char* msg = (char*)this->recvRaw(&len);
	msg[len + 1] = '\0';
	return msg;
}

#endif //__CONNECTION_HPP
