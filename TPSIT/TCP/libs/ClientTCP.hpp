#ifndef __CLIENT_TCP_HPP
#define __CLIENT_TCP_HPP

#include "../../libs/Address.hpp"
#include "SocketTCP.hpp"
#include "ClientConnection.hpp"
#include <unistd.h>

class ClientTCP : protected SocketTCP {
	private: ClientConnection* connection;
	public: ClientTCP();
			bool connect(Address);
			bool send(char*);
			char* recv();
			~ClientTCP();
};

ClientTCP::ClientTCP() : SocketTCP(){};

bool ClientTCP::connect(Address connAddress) {
	struct sockaddr_in conAddressSockaddr_in = connAddress.getSockaddr_in(); 
	int ret = ::connect(this->socketID, 
			  (struct sockaddr *)&conAddressSockaddr_in, 
			  (socklen_t)sizeof(struct sockaddr_in));
	connection = new ClientConnection(socketID, connAddress);
	return (ret != 0);
}

bool ClientTCP::send(char* msg) {
	return connection->send(msg);
}

char* ClientTCP::recv() {
	return connection->recv();
}

ClientTCP::~ClientTCP() {
	free(connection);
}


#endif //__SOCKET_TCP_HPP
