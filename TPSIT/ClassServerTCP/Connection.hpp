// Created by Andrea Delmastro, 2018-2019
//
// Header che rappresenta una classe Connection
// rappresentante una connessione TCP

#ifndef __CONNECTION_HPP
#define __CONNECTION_HPP

#include "../libs/Address.hpp"

#define MAX_MSG_SIZE 4096

class Connection {
private: int connectionID;
		 Address client;
public: Connection(int, Address);
		bool send(char*);
		char* recv();
		~Connection();
};

Connection::Connection(int connectionID, Address client) {
	this->connectionID = connectionID;
	this->client = client;
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

Connection::~Connection() {
	shutdown(this->connectionID, SHUT_RDWR);
}

#endif //__CONNECTION_HPP
