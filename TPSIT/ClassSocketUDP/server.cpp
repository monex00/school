#include "SocketUDP.hpp"
#include "../libs/UtilExt.h"

int main(int argc, char* argv[]) {
	if(argc != 3) {
		printf("USAGE %s PORT MESSAGE", argv[0]);
		return -1;
	}
	
	int port = atoi(argv[1]);
	char* msg = argv[2];
	
	SocketUDP mySelf(Address(IP_DHCP, port));
	
	Address mitt;
	
	char* buffer = mySelf.recvFromString(mitt);
	
	if(!buffer) error("ERRORE NELLA RICEZIONE DEL MESSAGGIO DAL CLIENT", -3);
	
	printf("MSG: %s\n", buffer);
	
	if(mySelf.sendToString(mitt, msg))
		error("ERRORE NELL'INVIO DEL MESSAGGIO AL CLIENT", -2);
		
	free(buffer);
	
	return 0;
}
