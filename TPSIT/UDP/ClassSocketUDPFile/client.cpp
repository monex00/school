#include "SocketUDP.hpp"
#include "../libs/UtilExt.h"

int main(int argc, char* argv[]) {
	if(argc != 5) {
		printf("USAGE %s IP PORT PATH_IN PATH_OUT", argv[0]);
		return -1;
	}
	
	char* ip = argv[1];
	int port = atoi(argv[2]);
	char* pathIn = argv[3];
	char* pathOut = argv[4];
		
	printf("[1]"); fflush(stdout);
	SocketUDP mySelf; 
	
	printf("[2]"); fflush(stdout);
	Address server(Address(ip, port));
	
	printf("[3]"); fflush(stdout);
	if(mySelf.sendToFile(server, pathIn))
		error("ERRORE NELL'INVIO DEL MESSAGGIO AL SERVER", -2);

	printf("[4]"); fflush(stdout);	
	Address mitt;
	
	printf("[5]"); fflush(stdout);
	int len;
	if(mySelf.recvFromFile(mitt, pathOut, &len)) error("ERRORE DI RICEZIONE DEL MESSAGGIO DAL SERVER", -4);
	
	printf("[6]"); fflush(stdout);
	return 0;
}
