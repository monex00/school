#include "ServerTCP.hpp"
#include <stdio.h>

#define header "HTTP/1.1 200 OK\nDate: Fri, 22 Feb 2019 10:50:37 GMT\nContent-Type: text/html; charset=UTF-8\nContent-Length: 22208\nConnection: keep-alive\nServer:test.andrea.it\n"

int main() {

	ServerTCP server(Address(IP_LOOPBACK, 5555));
	ServerConnection* connection = server.accept();
	connection->send((char*)header);
	connection->sendFile((char*)"/home/delmastro/school/TPSIT/TCP/pippo.html");
	server.closeConnection(connection);
}
