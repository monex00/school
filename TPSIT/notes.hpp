// Created by Andrea Delmastro, 2018-2019
//
// Header che rappresenta una classe ServerTCP
// per l'invio e la ricezione di stringhe in rete
// attraverso il prototcollo TCP

#ifndef __SERVER_TCP_HPP
#define __SERVER_TCP_HPP

#define MAX_MSG_SIZE 4096
#define MAX_CONN_NUM 50

#include "Connection.hpp"
#include <unistd.h>

class ServerTCP {
private: int socketID;
public: ServerTCP(Address);
		Connection* accept();
		
		~ServerTCP();
};

ServerTCP::ServerTCP(Address mySelf) {
	this->socketID = socket(AF_INET, SOCK_STREAM, 0);
	
	struct sockaddr_in addrMySelf = mySelf.getSockaddr_in();
	bind(this->socketID, 
	     (struct sockaddr *)&addrMySelf, 
	     (socklen_t)sizeof(struct sockaddr_in));
	     
	listen(this->socketID, MAX_CONN_NUM);
}

Connection* ServerTCP::accept() {
	struct sockaddr_in client;
	socklen_t size = (socklen_t) sizeof(struct sockaddr_in);
 	int connectionID = ::accept(this->socketID, 
 							    (struct sockaddr*)&client,
 							    (socklen_t*)&size);
 	return (connectionID < 0) ? NULL : new Connection(connectionID, Address(client));
}


ServerTCP::~ServerTCP() {
	close(this->socketID);
}

#endif //__SERVER_TCP_HPP

/*
 Creo una nuova classe SocketTCP, dalla quale ereditano ServerTCP e ClientTCP
 Le API socket() e close() vengono così chiamate una sola volta
 class SocketTCP {
	 private: int socketID;
	 public: SocketTCP(); -> socket()
	 		 -SocketTCP(); -> close()
 };
 ClientTCP e Connection utilizzano però entrambe le API send() e recv().
 Il metodo connect() di ClinetTCP crea allora una istanza di Connection
 e ClientTCP diventa una classe wrapper di Connection per i metodi send() e recv()
 che vengono richiamati dalla Connection.
 La classe Connection ha un distruttore che richiama shutdown(), ma shutdown() non
 può essere eseguito su un sockid (usato da ClientTCP).
 Creo allora due sottoclassi di Connection, una che esegue shutdown() l'altra che esegue
 close(). Le chiamo ServerConnection e ClientConnection. In ServerConnection ci sarà la
 chiamata a shutdown, non presente in ClientConnection.
 Il ServerTCP quando esegue ServerTCP::accept() ritornerà un ServerConnection, il ClientTCP
 quando esegue ClientTCP::connect() ritornerà un un ClientConnection.
 class ServerConnection {
	 public: -ServerConnection(); -> shutdown()
 };
 class ClientConnection {
	 public: -ClientConnection(); -> close()
 };
 ServetTCP deve mantenere una collezione di ServerConnection attive per poterle chiudere prima
 di distruggersi. La collezione deve essere una lista. Come lista possiamo utilizzare la list
 di stl (standard template laibrary) che contiene come elementi ServerConnection*.
 list<ServerConnection*>connections;
 Per lavorare con le liste utilizziamo i metodi pushfront() e remove().
 A grandi linee una classe list funziona così:
 struct Node {
	 int key;
	 struct Node* next; 
 };
 Dato che per definire struct Node ho utilizzato struct Node si tratta di una struttura
 ricorsiva, che lavorerà con funzioni ricorsive.
 Esempio di metodo per stamapre i valori di una lista bottom-top:
 void show(Node* first) {
	if(first) {
		show(first->next);
		printf("%d ", first->key);
	}
 }
 Esempio di metodo per stampare i valori di una lista top-bottom:
 void show(Node* first) {
	if(first) {
	 	printf("%d ", first->key);
		show(first->next);
	}
 }
 Esempio di metodo per eliminare i nodi di una lista:
 void deleteNode(Node* first) {
	if(first) {
		show(first->next);
		deleteNode(first);
	}
 }
 Esempio di metodo per aggiungere un nodo in testa alla lista:
 Node* add(Node* first, int key) {
	 Node* newNode = (Node*)malloc(sizeof(Node));
	 newNode->key = key;
	 newNode->next = first;
	 return(newNode);
 }
 Metodo accetta di ConnServer :
 ConnServer* ServerTCP::accetta() {
	ConnServer* ret;
	struct sockaddr client;
	int len = sizeof(struct sockaddr);
	int conn_id = accept(sockID, &client, &len);
	if(conn_id != -1) {
		 ret = new ServerConn(conn_id);
		 conns.pushfront(ret);
	} else {
		 ret = NULL;
	}
	return ret;
 } 
*/