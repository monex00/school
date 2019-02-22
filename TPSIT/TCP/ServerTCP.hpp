#ifndef __SERVER_TCP_HPP
#define __SERVER_TCP_HPP

#include "../libs/Address.hpp"
#include "SocketTCP.hpp"
#include "ServerConnection.hpp"
#include <unistd.h>
#include <list>

class ServerTCP : protected SocketTCP {
	private: std::list<ServerConnection*>connections;
	public: ServerTCP();
			ServerConnection* accept();
			~ServerTCP();
};

ServerTCP::ServerTCP() : SocketTCP(){};

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
		connections.push_front(newConnection);
		return newConnection;
	}
}


ServerTCP::~ServerTCP() {
	for(ServerConnection* connection : connections) free(connection);
	close(this->socketID);
}


#endif //__SERVER_TCP_HPP
