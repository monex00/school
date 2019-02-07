// Created by Andrea Delmastro, 2018-2019
//
// Header che rappresenta una classe ServerTCP
// per l'invio e la ricezione di stringhe in rete
// attraverso il prototcollo TCP

#ifndef __SERVER_TCP_HPP
#define __SERVER_TCP_HPP

#define MAX_MSG_SIZE 4096
#define MAX_CONN_NUM 50

#include "Connection.hpp"
#include <unistd.h>

class ServerTCP {
private: int socketID;
public: ServerTCP(Address);
		Connection* accept();
		
		~ServerTCP();
};

ServerTCP::ServerTCP(Address mySelf) {
	this->socketID = socket(AF_INET, SOCK_STREAM, 0);
	
	struct sockaddr_in addrMySelf = mySelf.getSockaddr_in();
	bind(this->socketID, 
	     (struct sockaddr *)&addrMySelf, 
	     (socklen_t)sizeof(struct sockaddr_in));
	     
	listen(this->socketID, MAX_CONN_NUM);
}

Connection* ServerTCP::accept() {
	struct sockaddr_in client;
	socklen_t size = (socklen_t) sizeof(struct sockaddr_in);
 	int connectionID = ::accept(this->socketID, 
 							    (struct sockaddr*)&client,
 							    (socklen_t*)&size);
 	return (connectionID < 0) ? NULL : new Connection(connectionID, Address(client));
}


ServerTCP::~ServerTCP() {
	close(this->socketID);
}


#endif //__SERVER_TCP_HPP
