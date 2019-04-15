#include "libs/ServerTCP.hpp"
#include <sqlite3.h>
#include <stdio.h>
#include <regex.h>
#include "../libs/UtilExt.h"

#define PROGRAM_DESC "== WEBSERVER v1.1 ==\n\n"

#define MAX_LINE_LEN 1000
#define MAX_FILE_LEN 100

#define DEFAULT_PAGE "/index.html"

#define DEFAULT_404_PAGE "/404.html"

// Default directories used to store html documents
#define DEFAULT_HTML_DIR "../TCP/html"
#define DEFAULT_HTML_TMP_DIR "../TCP/html/tmp/"

// HTML extension used to save temporary files
#define HTML_EXT ".html"

// Standard header string defined for code 200 and 404 answers
#define HEADER_200 "HTTP/1.1 200 OK\n\n"
#define HEADER_404 "HTTP/1.1 404 File Not Found\n\n"

// Regex that are used to extract useful information from the HTTP GET request
// and the custom tag interpreted by the CGI function
#define REGEX_HTTP_GET "^GET\\s([^\\?\\*<>|]+)\\sHTTP"
#define REGEX_DB "<SQL\\s+db=\"(.+)\"\\s+query=\"(.+);\"\\s*\\/>"
// Number of groups that are extracted from each regex previously defined
#define GROUPS_NUM_HTTP_GET 1
#define GROUPS_NUM_DB 2

char* getFileNameFromRqst(char* rqst);
char* getFilePath(char* pageName);
void* thread(void* arg);
char* gdi(char* filePath);
char * generateHTMLFromQuery(char *dbPath, char *query, FILE *fp);
int callback(void *param, int argc, char **argv, char **azColName);
int regexGroupExtractor(char* regex, char* textToMatch, char** res, int groupsNum);

struct thread_params {
	ServerTCP* serverSocket;
	ServerConnection* connection;
};

int main(int argc, char* argv[]) {

	// In caso di parametri sbagliati viene segnalato l'errore
	// all'utente
	if(argc != 2) {
		printf("USAGE: %s PORT\n", argv[0]);
		return -1;
	}
	
	printf(PROGRAM_DESC);
	
	// Conversione della porta in intero
	int port = atoi(argv[1]);

	// Apertura del socket
	Address mySelf(IP_LOOPBACK, port);
	ServerTCP serverSocket(mySelf);
	
	// Ciclo infinito
	while(true) {
		// Apertura della connessione
		ServerConnection* connection = serverSocket.accept();
		if(connection == nullptr) {
		    printf("[E] Unable to accept connection");
            break;
		}

		struct thread_params params {};
		params.serverSocket = &serverSocket;
		params.connection = connection;
		pthread_t thid;
		pthread_create(&thid, nullptr, thread, (void*)&params);
	}

}

char * generateHTMLFromQuery(char *dbPath, char *query, FILE *fp) {

    // Stampa sul file della prima riga della tabella
	fprintf(fp, (char*)"<table border=\"1\">");

	// Apertura del database
    sqlite3 *db;
    char* err_msg;

	sqlite3_open(dbPath, &db);
	// Esecuzione della query mediante chiamata alla funzione di callback
	sqlite3_exec(db, query, callback, (void*)fp, &err_msg);
	sqlite3_close(db);

	// Stampa su file dell'ultima riga della tabella
	fprintf(fp, (char*)"</table>");

    return err_msg;
}

int callback(void* param, int argc, char **argv, char **azColName) {

	FILE* fp = (FILE*)param;
	char* toWrite = (char*)malloc(MAX_LINE_LEN*sizeof(char));
	
	fprintf(fp, (char*)"<tr>");
	for (int i = 0; i < argc; i++) {
		sprintf(toWrite, "<th>%s</th>", azColName[i]);
		fprintf(fp, "%s", toWrite);
    }
    fprintf(fp, (char*)"</tr>");
    
    fprintf(fp, (char*)"<tr>");
    for (int i = 0; i < argc; i++) {
   		sprintf(toWrite, "<td>%s</td>", argv[i]);
		fprintf(fp, "%s", toWrite);
    }
    fprintf(fp, (char*)"</tr>");
    
    if(toWrite) free(toWrite);
    return 0;
}

char* gdi(char* filePath) {
	
	char* line = (char*) malloc(MAX_LINE_LEN * sizeof(char));
	char* tmpFileName = (char*)malloc(MAX_FILE_LEN * sizeof(char));
	
	FILE* orgFile = fopen(filePath, "r");
	// Generazione del percorso del file temporaneo dove verrà memorizzata la pagina
	// HTML da spedire nel formato PATH/id_thread.html
	sprintf(tmpFileName, "%s%ld%s", DEFAULT_HTML_TMP_DIR, pthread_self(), HTML_EXT);
	FILE* tmpFile = fopen(tmpFileName, "w");

	// Ciclo di scorrimento di tutte le linee presenti nel file
    size_t len;
    while(getline(&line, &len, orgFile) != -1) {
        // Array contenente le stringhe che vengono estrapolate come gruppi dalla regex
        char** groups = (char**)malloc(GROUPS_NUM_DB * sizeof(char*));

        // Esecuzione della query per estrarre i dati utili all'apertura e all'esecuzione
        // della query sul database
        if(regexGroupExtractor((char*)REGEX_DB, line, groups, GROUPS_NUM_DB) == 0){
            // groups[0] = percorso file
            // groups[1] = query
            char* retGenHTMLFromQuery = generateHTMLFromQuery(groups[0], groups[1], tmpFile);
            if(retGenHTMLFromQuery != nullptr) printf("[E] Error executing query : %s\n", retGenHTMLFromQuery);
        } else {
            fprintf(tmpFile, "%s" ,line);
        }

        free(groups);
    }

    free(line);
	fclose(orgFile);
	fclose(tmpFile);

	return tmpFileName;
}

void* thread(void* arg) {

	auto* params = (struct thread_params*) arg;
	ServerTCP* serverSocket = params->serverSocket;
	ServerConnection* connection = params->connection;

	char* rqst = connection->recv();
	if(rqst == nullptr) {
	    printf("[E] Error receiving request, aborting comunication ...\n");
	    pthread_exit((void*)-1);
	}

	char* pageName = getFileNameFromRqst(rqst);
	char* pagePath = getFilePath(pageName);
	char* tmpFilePath = nullptr;
	
	printf("[I] Connection established with host -> %s\n[I] Request Header :\n%s\n",/*connection->getConnectedAddress().toString(),*/ pageName, rqst);
	
	if(access(pagePath, R_OK) != -1) {
		tmpFilePath = gdi(pagePath);
    	connection->sendRaw((char*)HEADER_200, strlen((char*)HEADER_200));
    	connection->sendFile((char*)tmpFilePath);
		printf("[I] Sent %s page\n", pagePath);
	} else {
    	connection->sendRaw((char*)HEADER_404, strlen((char*)HEADER_404));
		connection->sendFile(getFilePath((char*)DEFAULT_404_PAGE));
		printf("[W] %s Not Found : 404 ERROR %s %s\n", pageName, pagePath, strerror(errno));
	}

	free(pageName);
	free(pagePath);
	free(tmpFilePath);
	free(rqst);

	serverSocket->closeConnection(connection);

	pthread_exit(nullptr);
}

char* getFileNameFromRqst(char* rqst) {
    // Array contenente le stringhe che vengono estrapolate come gruppi della regex
    char** groups = (char**)malloc(GROUPS_NUM_HTTP_GET * sizeof(char*));
    char* ret = nullptr;

    if(regexGroupExtractor((char*)REGEX_HTTP_GET, rqst, groups, GROUPS_NUM_HTTP_GET) == 0) {
        // Dato che un solo gruppo viene estrapolato dalla regex, viene ritornato un
        // duplicato di quella stringa
        if (strcmp(groups[0], "/") != 0) ret = strdup(groups[0]);
        else ret = strdup((char *) DEFAULT_PAGE);
    }

    free(groups);

    return ret;
}

// Funzione che permette, data una regex, un testo da confrontare,
// un vettore di stringhe e il numero di gruppi che si vogliono
// prelevare, di estrapolare tramite una regex i gruppi dal testo,
// ritornando gli stessi nel vettore di stringhe
int regexGroupExtractor(char* regex, char* textToMatch, char** res, int groupsNum) {

    regex_t preg;
    regmatch_t groupArray[groupsNum + 1];

    int ret = regcomp(&preg, regex, REG_EXTENDED);
    if(!ret) {
        ret = regexec(&preg, textToMatch, groupsNum + 1, groupArray, 0);

        if(!ret) {
            char sourceCopy[(strlen(textToMatch) + 1) * sizeof(char)];
            strcpy(sourceCopy, textToMatch);

            for(int i = 1; i < groupsNum + 1; i++) {
                sourceCopy[groupArray[i].rm_eo] = '\0';
                res[i - 1] = strdup(sourceCopy + groupArray[i].rm_so);
            }
        }
    }

    regfree(&preg);
    return ret;
}

char* getFilePath(char* pageName) {
    char* ret = (char*)malloc(MAX_FILE_LEN * sizeof(char));
	sprintf(ret, "%s%s", DEFAULT_HTML_DIR, pageName);
    return ret;
}
