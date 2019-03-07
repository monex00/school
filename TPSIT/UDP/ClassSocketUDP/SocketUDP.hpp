// Created by Andrea Delmastro, 2018-2019
//
// Header che rappresenta una classe SocketUDP
// per l'invio e la ricezione di stringhe in rete
// attraverso il prototcollo UDP

#ifndef __SOCKET_UDP_HPP
#define __SOCKET_UDP_HPP

#include "../libs/Address.hpp"
#include <unistd.h>

#define MAX_MSG_SIZE 4096

class SocketUDP {
private: int socketID;
public: SocketUDP();
		SocketUDP(Address);
		bool sendToString(Address, char*);
		char* recvFromString(Address&);
		bool sendToRaw(Address, char*, int);
		char* recvFromRaw(Address&, int*);
		
		~SocketUDP();
};

// Costruttore che non riceve nessun parametro.
// Viene inizializzato il socket con indirizzo demandato al DHCP
// e porta zero (wildcard per l'assegnazione della porta su base casuale
// da parte del sistema in base alle porte disponibili.
SocketUDP::SocketUDP() : SocketUDP(Address()) {}

// Costruttore che riceve un oggetto Address come parametro.
// Viene inizializzato il socket con l'indirizzo e la porta indicati
// all'interno dell'oggetto Address.
SocketUDP::SocketUDP(Address mySelf) {
	// La funzione socket() crea un endpoint di  comunicazione (socket)
	// e ritorna un descrittore di file che si riferisce 
	// all'endpoint appena creato.
	// AF_INET -> comunicazione attraverso il protocollo IPv4.
	// SOCK_DGRAM -> comunicazione non orientata alla connessione e
	//               inaffidabile.
	// 0 -> protocollo standard in base alle informazioni inserite 
	//      nei parametri precedenti (UDP).
	this->socketID = socket(AF_INET, SOCK_DGRAM, 0);

	// Creazione di una struttura sockaddr_in tramite il metodo
	// Address::getSockaddr_in().
	// La struttura così creata rappresenta le proprietà
	// dell'oggetto addrMySelf.
	struct sockaddr_in addrMySelf = mySelf.getSockaddr_in();
	
	// La funzione bind() assegna l'indirizzo rappresentato nella struttura
	// sockaddr al socket cui si riferisce il descrittore socketID.
	// socketID -> descrittore del socket (endpoint) creato precedentemente.
	// addrMySelf -> struttura sockaddr_in castata in sockaddr * per rispettare
	//               la parametrizzazione della funzione e rappresentante
	//               indirizzo e porta di comunicazione.
	// sizeof(struct sockaddr_in) -> lunghezza della struttura puntata dal 
	//                               parametro precedente.
	int bindRetVal = bind(socketID, 
	                      (struct sockaddr *)&addrMySelf, 
	                      (socklen_t)sizeof(struct sockaddr_in));
}

// Metodo che invia una stringa msg ad un destinatario descritto nell'oggetto
// destAddress.
// @return true/false se la procedura ha avuto successo o meno. 
bool SocketUDP::sendToString(Address destAddress, char* msg) {
	return SocketUDP::sendToRaw(destAddress, msg, strlen(msg) + 1);
}

// Metodo che ritorna una stringa ricevuta da un nodo di rete.
// Il mittente viene indicato nell'oggetto sendAddress passato per referenza.
// @return la stringa ricevuta, null terminata.
char* SocketUDP::recvFromString(Address& sendAddress) {
	int len;
	char* msg = SocketUDP::recvFromRaw(sendAddress, &len);
	// Per sicurezza al termine della stringa viene aggiunto il carattere di fine
	// stringa.
	msg[len + 1] = '\0';
	return msg;
}

bool SocketUDP::sendToRaw(Address destAddress, char* msg, int len) {
	bool retVal = false;
	
	// Conversione dell'oggetto destAddress in una struttura sockaddr_in 
	// corrispondente
	struct sockaddr_in destAddressSockaddr_in = destAddress.getSockaddr_in();
	
	// La funzione sendto() viene utilizzata per inviare un messaggio ad un altro socket.
	// socketID -> descrittore del socket (endpoint) creato precedentemente.
	// msg -> puntatore al messaggio da inviare.
	// strlen(msg) + 1 -> lunghezza del messaggio in byte.
	// 0 -> flag standard.
	// destAddressSockaddr_in -> indirizzo del destinatario (client) castato a 
	// 							 sockaddr* per rispettare la parametrizzazione della funzione.
	// sizeof(struct sockaddr_in) -> lunghezza della struttura sockaddr puntata dal
	//                               parametro precedente castata per rispettare
	//                               la parametrizzazione della funzione.
	int sendtoRetVal = sendto(socketID, 
							  msg, 
							  len, 
							  0, 
							  (struct sockaddr *)&destAddressSockaddr_in, 
							  (socklen_t)sizeof(struct sockaddr_in));
	
	// La funzione sendto() ritorna il numero di byte inviati in caso di successo.
	if(sendtoRetVal != len) retVal = true;
	return retVal;
}

char* SocketUDP::recvFromRaw(Address& sendAddress, int* len) {
	// Creazione di un buffer di lunghezza massima del messaggio più 1 per il
	// carattere di fine stringa.
	char* buffer = (char*)malloc((MAX_MSG_SIZE + 1) * sizeof(char));
	
	// Struttura che conterrà l'indirizzo del mittente del messaggio.
	struct sockaddr_in sendAddressSockaddr_in;
	int addrLen = sizeof(struct sockaddr_in);
	
	// La funzione recvfrom() riceve un messaggio da un socket. 
	// socketID -> descrittore del socket (endpoint) creato precedentemente.
	// buffer -> buffer dove viene immagazzinata la stringa ricevuta dal client.
	// MAX_MSG_SIZE -> lunghezza massima del messaggio (dimensione del buffer).
	// 0 -> flag standard.
	// sendAddressSockaddr_in -> indirizzo del mittente (client) castato a sockaddr* per 
	//             				 rispettare la parametrizzazione della funzione.
	// addrLen -> lungezza della struttura passata come parametro (in questo caso
	//            addrClient). E' un valore di ritorno della funzione: nel caso in cui
	//            il buffer risultasse troppo piccolo il valore ritornato in questo
	//            parametro risulterà essere maggiore di quello passatto alla chiamata.
	int recvfromRetVal = recvfrom(socketID,
	                              buffer,
	                              MAX_MSG_SIZE,
	                              0,
	                              (struct sockaddr *)&sendAddressSockaddr_in,
	                              (socklen_t *)&addrLen);
	                              
	sendAddress.setSockaddr_in(sendAddressSockaddr_in);
	                              
	// In caso di errore la funzione recvfrom() ritorna -1. 
	if(recvfromRetVal < 0) return NULL;
	// Altrimenti ritorna il numero di byte ricevuti
	else *len = recvfromRetVal;
	
	return buffer;
}

SocketUDP::~SocketUDP() {
	close(this->socketID);
}


#endif //__SOCKET_UDP_HPP
