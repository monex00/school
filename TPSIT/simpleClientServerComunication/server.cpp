// Created by Andrea Delmastro, 2018-2019
//
// Programma server per l'implementazione
// di una comunicazione UDP in C.

#include "Address.hpp"
#include "UtilExt.h"

#define PORT_MYSELF 7000
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
	
	// Creazione di un oggetto Address con risoluzione di indirizzo
	// demandata al server DHCP e porta PORT_MYSELF.
	Address mySelf(IP_DHCP, PORT_MYSELF);
	
	// Creazione di una struttura sockaddr_in tramite il metodo
	// Address::getSockaddr_in().
	// La struttura così creata rappresenta le proprietà
	// dell'oggetto mySelf.
	struct sockaddr_in addrMySelf = mySelf.getSockaddr_in();
	
	// La funzione bind() assegna l'indirizzo rappresentato nella struttura
	// sockaddr al socket cui si riferisce il descrittore socketId.
	// socketId -> descrittore del socket (endpoint) creato precedentemente.
	// addrMySelf -> struttura sockaddr_in castata in sockaddr * per rispettare
	//               la parametrizzazione della funzione e rappresentante
	//               indirizzo e porta di comunicazione.
	// sizeof(struct sockaddr_in) -> lunghezza della struttura puntata dal 
	//                               parametro precedente.
	int bindRetVal = bind(socketId, 
	                      (struct sockaddr *)&addrMySelf, 
	                      (socklen_t)sizeof(struct sockaddr_in));
	
	// In caso di errore la funzione bind() ritorna -1.           
	if(bindRetVal < 0) error("Error assigning the address to the socket.", -2);
	
	// Creazione di un buffer di lunghezza massima del messaggio più 1 per il
	// carattere di fine stringa.
	char buffer[MAX_MSG_SIZE + 1];
	
	// Struttura che conterrà l'indirizzo del mittente del messaggio (il client).
	struct sockaddr_in mittAddr;
	int addrLen = sizeof(struct sockaddr_in);
	
	// La funzione recvfrom() riceve un messaggio da un socket. 
	// socketId -> descrittore del socket (endpoint) creato precedentemente.
	// buffer -> buffer dove viene immagazzinata la stringa ricevuta dal client.
	// MAX_MSG_SIZE -> lunghezza massima del messaggio (dimensione del buffer).
	// 0 -> flag standard
	// mittAddr -> indirizzo del mittente (client) castato a sockaddr* per 
	//             rispettare la parametrizzazione della funzione.
	// addrLen -> lungezza della struttura passata come parametro (in questo caso
	//            mittAddr). E' un valore di ritorno della funzione: nel caso in cui
	//            il buffer risultasse troppo piccolo il valore ritornato in questo
	//            parametro risulterà essere maggiore di quello passatto alla chiamata.
	int recvfromRetVal = recvfrom(socketId,
	                              buffer,
	                              MAX_MSG_SIZE,
	                              0,
	                              (sockaddr*)&mittAddr,
	                              (socklen_t *)&addrLen);
	                              
	// In caso di errore la funzione recvfrom() ritorna -1. 
	if(recvfromRetVal < 0) error("Error receving message from client.", -3);
	
	// In caso di successo la funzione recvfrom() ritorna il numero di byte ricevuti.
	// Sulla base di ciò, per sicurezza, viene aggiunto il carattere di fine stringa al
	// buffer.
	buffer[recvfromRetVal] = '\0';
	
	// Creazione di un oggetto Address partendo dalla struttura mittAddr inizializzata
	// in recvfrom().
	Address mittente(mittAddr);
	
	// Acquisizione della stringa rappresentante l'indirizzo (IPv4:porta) del mittente 
	// (client).
	char* mittToStr = mittente.toString();
	
	// Stampa a video delle informazioni sul mittente.
	printf("From: %s\t\"%s\"\n", mittToStr, buffer);
	
	free(mittToStr);
	
	// Creazione messaggio da inviare al client
	char msg[] = "I'm your server";
	
	       
	return 0;
}
