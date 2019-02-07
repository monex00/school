// Created by Andrea Delmastro, 2018-2019
//
// Header che rappresenta una classe ClientTCP
// per l'invio e la ricezione di stringhe in rete
// attraverso il prototcollo TCP

#ifndef __SOCKET_TCP_HPP
#define __SOCKET_TCP_HPP

#include "../libs/Address.hpp"
#include <unistd.h>

#define MAX_MSG_SIZE 4096

class ClientTCP {
private: int socketID;
public: ClientTCP();
		ClientTCP(Address);
		bool connect(Address);
		bool send(char*);
		char* recv();
		
		~ClientTCP();
};

ClientTCP::ClientTCP() : ClientTCP(Address()) {}

ClientTCP::ClientTCP(Address mySelf) {
	this->socketID = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in addrMySelf = mySelf.getSockaddr_in();
	int bindRetVal = bind(socketID, 
	                      (struct sockaddr *)&addrMySelf, 
	                      (socklen_t)sizeof(struct sockaddr_in));
}

bool ClientTCP::connect(Address connAddress) {
	struct sockaddr_in conAddressSockaddr_in = connAddress.getSockaddr_in(); 
	return ::connect(this->socketID, 
				  (struct sockaddr *)&conAddressSockaddr_in, 
				  (socklen_t)sizeof(struct sockaddr_in));
}

bool ClientTCP::send(char* msg) {
	int sendRet = ::send(this->socketID, msg, strlen(msg) + 1, 0);
	return (sendRet == (strlen(msg) + 1)) ? true : false;
}

char* ClientTCP::recv() {
	char* buffer = (char*)malloc((MAX_MSG_SIZE + 1) * sizeof(char));
	int recvRet = ::recv(this->socketID, buffer, MAX_MSG_SIZE, 0);
	buffer[MAX_MSG_SIZE + 1] = '\0';
	return (recvRet == 0) ? buffer :  NULL;
}



#endif //__SOCKET_TCP_HPP
