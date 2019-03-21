#ifndef __SERVER_CONNECTION_HPP
#define __SERVER_CONNECTION_HPP

#include "../../libs/Address.hpp"
#include "Connection.hpp"
#include <unistd.h>

#define MAX_MSG_SIZE 4086

class ServerConnection : public Connection {
	public: ServerConnection(int, Address);
			~ServerConnection();
};

ServerConnection::ServerConnection(int connectionID, Address connectedAddress) : Connection(connectionID, connectedAddress){
	connectedAddress.getIp(); //Va rimosso da qua, ma senza questo non funziona -\_(0.0)_/-
};

ServerConnection::~ServerConnection() {
	shutdown(this->connectionID, SHUT_RDWR);
}

#endif //__SERVER_CONNECTION_HPP
