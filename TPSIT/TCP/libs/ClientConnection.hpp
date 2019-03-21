#ifndef __CLIENT_CONNECTION_HPP
#define __CLIENT_CONNECTION_HPP

#include "../../libs/Address.hpp"
#include "Connection.hpp"
#include <unistd.h>

#define MAX_MSG_SIZE 4086

class ClientConnection : public Connection {
	public: ClientConnection(int, Address);
			~ClientConnection();
};

ClientConnection::ClientConnection(int connectionID, Address connectedAddress) : Connection(connectionID, connectedAddress){};

ClientConnection::~ClientConnection() {
	close(this->connectionID);
}

#endif //__CLIENT_CONNECTION_HPP
