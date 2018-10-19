# TODO

* close() nella remove
* passaggio per referenza di SocketUDP in tutti i metodi che usano SocketUDP
* Creazione di inviaRaw() e ricevi()
  * riceviRaw() ritorna anche il numero di byte ricevuti e non aggiunge la '\0' al termine 
  * inviaRaw() riceve come parametro il numero di byte da inviare e non li calcola basandosi sulla posizione dell '\0'
