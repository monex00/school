// Created by Andrea Delmastro, 2018-2019
//
// Header che rappresenta una classe Address
// da implementare per comunicazioni TCP/UDP
// fra host in C.

#ifndef __ADDRESS_HPP
#define __ADDRESS_HPP

#define IP_DHCP "0.0.0.0"
#define IP_LOOPBACK "127.0.0.1"
#define PORT_ZERO 0
#define LEN_ADDRESS_STR 25

#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>

// Classe che rappresenta un indirizzo di rete composto
// da IPv4 e porta.
class Address {
private: char* ip = NULL;
		 int port;
		 static pthread_mutex_t mutex;
public:  Address(const char*, int);
		 Address();
		 Address(const Address&);
		 Address(struct sockaddr_in);
		
		 char* getIp();
		 int getPort();
		 void setIp(const char*);
		 void setPort(int);
		 
		 struct sockaddr_in getSockaddr_in();
		 void setSockaddr_in(struct sockaddr_in);
		
		 char* toString();
		 
		 ~Address();
};

// Inizializzazione del mutex attraverso la macro apposita per
// l'inizializzazione dei pthread_mutex statici.
// Non viene eseguito nessun controllo degli errori.
// Il mutex viene inizializzato a valori di default.
// Un mutex creato in questo modo non necessita la chiamata alla
// funzione di eliminazione.
pthread_mutex_t Address::mutex = PTHREAD_MUTEX_INITIALIZER;

// Costruttore che riceve come parametro un indirizzo
// IPv4 e una porta.
// L'indirizzo IPv4 viene copiato attraverso un'allocazione
// dinamica.
Address::Address(const char* ip, int port) {
	setIp(ip);
	this->port = port;
}

// Costruttore che non riceve alcun parametro: indirizzo
// IPv4 e porta vengono asseganti automaticamente: per
// l'indirzzo IPv4 si demanda la risoluzione al server 
// DHCP. Per la porta viene demandata l'assegnazione al SO.
Address::Address() : Address(IP_DHCP, PORT_ZERO){}

// Costruttore per copia: esegue la copia di un altro oggetto
// della classe indirizzo.
Address::Address(const Address& address) : Address(address.ip, address.port) {
}

// Costruttore che riceve come parametro una struct 
// sockaddr_in (definita in sys/socket.h) e inizializa 
// l'oggetto sulla base delle informazioni contenute nella
// struttura.
Address::Address(struct sockaddr_in address) {
	setSockaddr_in(address);
}

// Getter che ritorna una copia allocata dinamicamente
// dell'indirizzo IPv4 sotto forma di stringa.
// @return l'IP assegnato all'oggetto.
char* Address::getIp() {
	return strdup(this->ip);
}

// Getter che ritorna la porta sotto forma di intero
// @return la porta assegnata all'oggetto.
int Address::getPort() {
	return (this->port);
}

// Setter che imposta l'indirizzo IPv4 ricevendo come parametro
// la stringa rappresentante l'indirizzo stesso. Viene prima 
// liberata la memoria dal precedente indirizzo IPv4, e poi 
// effettuata la copia mediante allocazione dinamica.
void Address::setIp(const char* ip) {
	free(this->ip);
	this->ip = strdup(ip);
}

// Setter che imposta la porta ricevendo come parametro un intero
// rappresentante il numero di porta stesso.
void Address::setPort(int port) {
	this->port = port;
}

// Getter che ritorna una struttura sockaddr_in compilata in relazione
// alle informazioni contenute nell'oggetto corrente. 
struct sockaddr_in Address::getSockaddr_in() {
	struct sockaddr_in retAddress;
	
	// AF_INET -> famiglia di indirizzi IPv4.
	retAddress.sin_family = AF_INET;
	// htnos() converte la porta dall'ordine di byte dell'host
	// all'ordine di byte della rete.
	retAddress.sin_port = htons(this->port);
	// inet_aton() converte l'indirizzo internet dal formato 
	// IPv4 con notazione a.b.c.d (es 192.168.0.1) in forma binaria
	// nell'ordine di byte della rete.
	inet_aton(this->ip, &retAddress.sin_addr);
	// Il campo sin_zero viene inizializzato a 8x0 per raggiungere
	// la lunghezza standard del record sockaddr.
	// L'operazione è opzionale e non necessaria.
	for(int i = 0; i < 8; i++) retAddress.sin_zero[i] = 0;
	
	return retAddress;
}

// Setter che imposta indirizzo e porta ricevendo come paramentro una
// struct sockaddr_in precompilata.
// La funzione inet_ntoa() non è thread safe: viene utilizzato un semaforo
// per sincronizzare in mutua esclusione tutti gli oggetti di classe
// Address che tentassero di utilizzare la funzione in contemporanea.
void Address::setSockaddr_in(struct sockaddr_in address) {
	// Accesso all'area critica.
	pthread_mutex_lock(&this->mutex);
	// inet_ntoa() rappresenta la funzione complementare a inet_aton().
	setIp(inet_ntoa(address.sin_addr));
	// Uscita dall'area critica.
	pthread_mutex_unlock(&this->mutex);
	// ntohs() rappresenta la funzione complementare a htons().
	this->port = ntohs(address.sin_port);
}

// Metodo che ritorna una stringa allocata dinamicamente rappresentate
// testualmente l'oggetto della classe Address.
char* Address::toString() {
	char temp[LEN_ADDRESS_STR];
	sprintf(temp, "[%s:%d]", this->ip, this->port);
	return strdup(temp);
}

// Distruttore che libera la memoria dalla stringa rappresentante
// l'indirizzo IPv4 precedentemente allocata dinamicamente e il 
Address::~Address() {
	free(this->ip);
}

#endif //__ADDRESS_HPP
