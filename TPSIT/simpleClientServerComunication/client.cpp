// Created by Andrea Delmastro, 2018-2019
//
// Programma client per l'implementazione
// di una comunicazione UDP in C.

#include "Address.hpp"
#include "UtilExt.h"
#include <unistd.h>

#define PORT_SERVER 7000
#define MAX_MSG_SIZE 4096

int main(int argc, char* argv[]) {

	// La funzione socket() crea un endpoint di  comunicazione (socket)
	// e ritorna un descrittore di file che si riferisce 
	// all'endpoint appena creato.
	// AF_INET -> comunicazione attraverso il protocollo IPv4.
	// SOCK_DGRAM -> comunicazione non orientata alla connessione e
	//               inaffidabile.
	// 0 -> protocollo standard in base alle informazioni inserite 
	//      nei parametri precedenti (UDP).
	int socketId = socket(AF_INET, SOCK_DGRAM, 0);
	
	// In caso di errore la funzione socket() ritorna -1.
	if(socketId < 0) error("Error creating edpoint for comunication.", -1);
	
	// Creazione di un oggetto Address rappresentante il server con indirizzo 
	// di loopback e porta PORT_SERVER.
	Address server(IP_LOOPBACK, PORT_SERVER);
	
	// Creazione di un oggetto Address rappresentante il server stesso
	// con indirizzo di loopback e porta PORT_MYSELF.
	struct sockaddr_in addrServer = server.getSockaddr_in();
	
	// Creazione messaggio da inviare al server.
	char msg[] = "I'm your client";
	
	// La funzione sendto() viene utilizzata per inviare un messaggio ad un altro socket.
	// socketId -> descrittore del socket (endpoint) creato precedentemente.
	// msg -> puntatore al messaggio da inviare.
	// strlen(msg) + 1 -> lunghezza del messaggio in byte.
	// 0 -> flag standard.
	// addrServer -> indirizzo del destinatario (server) castato a sockaddr* per 
	//               rispettare la parametrizzazione della funzione.
	// sizeof(struct sockaddr_in) -> lunghezza della struttura sockaddr puntata dal
	//                               parametro precedente castata per rispettare
	//                               la parametrizzazione della funzione.
	int sendtoRetVal = sendto(socketId, msg, strlen(msg) + 1, 0, (struct sockaddr*)&addrServer, (socklen_t)sizeof(struct sockaddr_in));
	
	// La funzione sendto() ritorna il numero di byte inviati in caso di successo.
	if(sendtoRetVal != strlen(msg) + 1) error("Error sending message to server", -2);
	
	// Creazione di un buffer di lunghezza massima del messaggio più 1 per il
	// carattere di fine stringa.
	char buffer[MAX_MSG_SIZE + 1];
	
	// La funzione recv() riceve un messaggio da un socket. 
	// socketId -> descrittore del socket (endpoint) creato precedentemente.
	// buffer -> buffer dove viene immagazzinata la stringa ricevuta dal client.
	// MAX_MSG_SIZE -> lunghezza massima del messaggio (dimensione del buffer).
	// 0 -> flag standard.
	int recvRetVal = recv(socketId, buffer, MAX_MSG_SIZE, 0);
	
	// In caso di errore la funzione recv() ritorna -1. 
	if(recvRetVal < 0) error("Error receving message from server.", -3);
	
	// In caso di successo la funzione recv() ritorna il numero di byte ricevuti.
	// Sulla base di ciò, per sicurezza, viene aggiunto il carattere di fine stringa al
	// buffer.
	buffer[recvRetVal] = '\0';
	
	// Acquisizione della stringa rappresentante l'indirizzo (IPv4:porta) del server.
	char* serverToStr = server.toString();
	
	// Stampa a video delle informazioni sul server e la stringa ricevuta.
	printf("From: %s\t\"%s\"\n", serverToStr, buffer);
	
	free(serverToStr);
	
	// Chiusura del socket.
	close(socketId);
}
