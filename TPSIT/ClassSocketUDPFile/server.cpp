#include "SocketUDP.hpp"
#include "../libs/UtilExt.h"

int main(int argc, char* argv[]) {
	if(argc != 4) {
		printf("USAGE %s PORT PATH_IN PATH_OUT", argv[0]);
		return -1;
	}
	
	int port = atoi(argv[1]);
	char* pathIn = argv[2];
	char* pathOut = argv[3];
	
	SocketUDP mySelf(Address(IP_DHCP, port));
	
	Address mitt;
	
	int len;
	if(mySelf.recvFromFile(mitt, pathOut, &len)) error("ERRORE NELLA RICEZIONE DEL MESSAGGIO DAL CLIENT", -3);
	
	if(mySelf.sendToFile(mitt, pathIn))
		error("ERRORE NELL'INVIO DEL MESSAGGIO AL CLIENT", -2);
	
	return 0;
}
