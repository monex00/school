#include "SocketUDP.hpp"
#include "../libs/UtilExt.h"

int main(int argc, char* argv[]) {
	if(argc != 4) {
		printf("USAGE %s IP PORT MESSAGE", argv[0]);
		return -1;
	}
	
	char* ip = argv[1];
	int port = atoi(argv[2]);
	char* msg = argv[3];
	
	SocketUDP mySelf; 
	
	Address server(Address(ip, port));
	
	if(mySelf.sendToString(server, msg))
		error("ERRORE NELL'INVIO DEL MESSAGGIO AL SERVER", -2);
		
	Address mitt;
	
	char* buffer = mySelf.recvFromString(mitt);
	
	if(!buffer) error("ERRORE DI RICEZIONE DEL MESSAGGIO DAL SERVER", -4);
	
	printf("MSG: %s\n", buffer);
	
	free(buffer);
	
	return 0;
}
