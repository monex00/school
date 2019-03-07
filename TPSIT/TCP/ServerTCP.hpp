#ifndef __SERVER_TCP_HPP
#define __SERVER_TCP_HPP

#include "../libs/Address.hpp"
#include "SocketTCP.hpp"
#include "ServerConnection.hpp"
#include <unistd.h>
#include <list>

class ServerTCP : protected SocketTCP {
	private: std::list<ServerConnection*>*connections;
			 void closeAllConnections();
	public: ServerTCP(Address mySelf);
			ServerConnection* accept();
			void closeConnection(ServerConnection*);
			void sendBroadcast(char*);
};

ServerTCP::ServerTCP(Address mySelf) : SocketTCP(){
	connections = new std::list<ServerConnection*>();
	
	struct sockaddr_in addrMySelf = mySelf.getSockaddr_in();
	bind(socketID, (struct sockaddr *)&addrMySelf,
	               (socklen_t)sizeof(struct sockaddr_in));
	               
	listen(this->socketID, 50);
	
};

ServerConnection* ServerTCP::accept() {
	struct sockaddr_in client;
	socklen_t size = (socklen_t) sizeof(struct sockaddr_in);
 	int connectionID = ::accept(this->socketID, 
 							    (struct sockaddr*)&client,
 							    (socklen_t*)&size);
 	if(connectionID < 0) {
		return NULL;
	} else {
		ServerConnection* newConnection = new ServerConnection(connectionID, Address(client));
		connections->push_front(newConnection);
		return newConnection;
	}
}

void ServerTCP::closeConnection(ServerConnection* connection) {
	connections->remove(connection);
}

void ServerTCP::closeAllConnections() {
	for (std::list<ServerConnection*>::const_iterator iterator = connections->begin(), end = connections->end(); iterator != end; ++iterator) {
    	this->closeConnection(*iterator);
	}
}

void ServerTCP::sendBroadcast(char* msg) {
	// MAI TESTATO
	for (std::list<ServerConnection*>::const_iterator iterator = connections->begin(), end = connections->end(); iterator != end; ++iterator) {
    	(*iterator)->send(msg);
	}
}

#endif //__SERVER_TCP_HPP
