#include "Address.hpp"
#include <string.h>

#define IP_TEST "127.0.0.1"
#define PORT_TEST 88

int main(int argc, char* argv[]) {

	/* ======== */
	
	printf("* Test costruttori:\n");
	
	char* ipStringTest = strdup(IP_TEST);
	char* ip;
	
	printf("\tAddress()\t");
	Address indirizzoTest1;
	ip = indirizzoTest1.getIp();
	if(strcmp(ip, IP_DHCP) == 0 && indirizzoTest1.getPort() == PORT_ZERO)
		printf("[ok]\n");
	else
		printf("[x]\n");
	free(ip);
	
	printf("\tAddress(char* ip, int port)\t");
	Address indirizzoTest2(ipStringTest, PORT_TEST);
	ip = indirizzoTest2.getIp();
	if(strcmp(indirizzoTest2.getIp(), IP_TEST) == 0 && indirizzoTest2.getPort() == PORT_TEST)
		printf("[ok]\n");
	else
		printf("[x]\n");
	free(ip);
		
	printf("\tAddress(char* ip, int port)\t");
	Address indirizzoTest3(ipStringTest, PORT_TEST);
	ip = indirizzoTest3.getIp();
	if(strcmp(ip, IP_TEST) == 0 && indirizzoTest3.getPort() == PORT_TEST)
		printf("[ok]\n");
	else
		printf("[x]\n");
	free(ip);
	
	printf("\tAddress(struct sockaddr_in)\t");
	struct sockaddr_in sockaddrTest;
	sockaddrTest.sin_family = AF_INET;
	inet_aton(ipStringTest, &sockaddrTest.sin_addr);
	sockaddrTest.sin_port = htons(PORT_TEST);
	Address indirizzoTest8(sockaddrTest);
	ip = indirizzoTest8.getIp();
	if(strcmp(ip, IP_TEST) == 0 && indirizzoTest8.getPort() == PORT_TEST) 
			printf("[ok]\n");
	else
		printf("[x]\n");
	free(ip);
		
	/* ======== */
	
	printf("* Test getters/setters\n");
	
	printf("\tgetPort() / setPort()\t");
	Address indirizzoTest4;
	indirizzoTest4.setPort(PORT_TEST);
	if(indirizzoTest4.getPort() == PORT_TEST)
		printf("[ok]\n");
	else
		printf("[x]\n");
		
	printf("\tgetIp() / setIp()\t");
	Address indirizzoTest5;
	indirizzoTest5.setIp(ipStringTest);
	ip = indirizzoTest5.getIp();
	if(strcmp(ip, IP_TEST) == 0)
		printf("[ok]\n");
	else
		printf("[x]\n");
	free(ip);
	
	printf("\tsetSockaddr_in()\t");
	Address indirizzoTest6;
	indirizzoTest6.setSockaddr_in(sockaddrTest);
	ip = indirizzoTest6.getIp();
	if(strcmp(ip, IP_TEST) == 0 && indirizzoTest6.getPort() == PORT_TEST)
		printf("[ok]\n");
	else
		printf(" [x]\n");
	
	printf("\tgetSockaddr_in()\t");
	Address indirizzoTest7(ipStringTest, PORT_TEST);
	sockaddrTest = indirizzoTest7.getSockaddr_in();
	if(strcmp(inet_ntoa(sockaddrTest.sin_addr), ipStringTest) == 0 && ntohs(sockaddrTest.sin_port) == PORT_TEST)
		printf("[ok]\n");
	else
			printf(" [x]\n");
	/* ======== */
	
	
	free(ipStringTest);
	
	return 0;
}
