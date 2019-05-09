# Appunti di programmazione funzionale 
## Programmazione funzionale
* Il flusso di esecuzione del programma assume la forma di una serie di valutazioni di funzioni
* Una funzione pura è una funzione che dato un determinato input restituisce sempre il medesimo output senza effetti collateral
* Una funzione che accetta come parametro una funzione o che ritorna una funzione è detta funzione di ordine superiore
## JavaScript
### Oggetti - Funzioni
* In js le funzioni sono oggetti function
* Le funzioni ritornano sempre un valore undefined
* Essendo oggetti le funzioni possono essere assegnate a variabili
* In js tutto ciò che non è una primitiva è un oggetto
* Le funzioni possono essere passate come parametri ad una funzione
* Una funzione si distingue da un oggetto perchè può essere invocata
* Qualsiasi oggetto può essere modificato dinamicamente
### Event Loop
* js è single thread
* L'I/O non è bloccante
* Esiste un call stack che registra tutte le chiamate a funzione: push al richiamo della funzione, pop al ritorno di un valore
* Ad esempio posso parallelizzare più richieste http senza che nessuna di questa blocchi l'esecuzione del codice
* La sincronizzazione avviene tramite callback, al termine dell'esecuzione viene richiamata una funzione di callback
#### Promise
* La promise ci permette di gestire un valore non ancora noto alla sua creazione
* Inserendo nella promise una funzione è possibile gestire il valore di ritorno tramite il metodo then()
#### Async / Await
* Tramite i costrutti awayt e async è possibile bloccare l'esecuzione di funzioni asincrone