#include "libs/ServerTCP.hpp"
#include <sqlite3.h>
#include <stdio.h>
#include <regex.h>
#include "../libs/UtilExt.h"

#define PROGRAM_DESC "== WEBSERVER v1.0 ==\n\n"

#define DB_PATH "db"

#define MAX_LINE_LEN 1000
#define TMP_FILE_NAME_LEN 100

#define DEFAULT_PAGE "/index.html"

#define SQL_TAG "<SQL"

#define DEFAULT_HTML_DIR "html"
#define DEFAULT_HTML_TMP_DIR "html/tmp/"
#define HTML_EXT ".html"

#define HEADER_200 "HTTP/1.1 200 OK\n\n"
#define HEADER_404 "HTTP/1.1 404 File Not Found\n\n"

#define REGEX "^GET\\s([^\\?\\*<>|]+)\\sHTTP"
#define MAX_GROUPS 2

char* getFileNameFromRqst(char* rqst);
char* getFilePath(char* pageName);
void* thread(void* arg);
char* gdi(char* filePath);
void generateHTMLFromQuery(char* query);
int callback(void *NotUsed, int argc, char **argv, char **azColName);

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
		struct thread_params params;
		params.serverSocket = &serverSocket;
		params.connection = connection;
		pthread_t thid;
		pthread_create(&thid, NULL, thread, (void*)&params);
	}

}

void generateHTMLFromQuery(char* query, FILE* fp) {
	sqlite3 *db;
	sqlite3_stmt* res;
	char* err_msg = NULL;
	
	fprintf(fp, (char*)"<table border=\"1\">");
	sqlite3_open((char*)DB_PATH, &db);
	sqlite3_exec(db, query, callback, (void*)fp, &err_msg);
	sqlite3_close(db);
	
	fprintf(fp, (char*)"</table>");
}

int callback(void* param, int argc, char **argv, char **azColName) {

	FILE* fp = (FILE*)param;
	char* toWrite = (char*)malloc(MAX_LINE_LEN*sizeof(char));
	
	fprintf(fp, (char*)"<tr>");
	for (int i = 0; i < argc; i++) {
		sprintf(toWrite, "<th>%s</th>", azColName[i]);
		fprintf(fp, toWrite);
    }
    fprintf(fp, (char*)"</tr>");
    
    fprintf(fp, (char*)"<tr>");
    for (int i = 0; i < argc; i++) {
   		sprintf(toWrite, "<td>%s</td>", argv[i]);
		fprintf(fp, toWrite);
    }
    fprintf(fp, (char*)"</tr>");
    
    if(toWrite) free(toWrite);
    return 0;
}

char* gdi(char* filePath) {	
	FILE* orgFile = NULL;
	FILE* tmpFile = NULL;
	char* line = NULL;
	char* occ = NULL;
	char* query = NULL;
	char* tmpFileName = NULL;
	size_t len;
	ssize_t resRead;
	
	line = (char*) malloc(MAX_LINE_LEN * sizeof(char));
	
	tmpFileName = (char*)malloc(TMP_FILE_NAME_LEN * sizeof(char));
	
	orgFile = fopen(filePath, "r");
	sprintf(tmpFileName, "%s%ld%s", DEFAULT_HTML_TMP_DIR, pthread_self(), HTML_EXT);
	tmpFile = fopen(tmpFileName, "w");
	
	while((resRead = getline(&line, &len, orgFile)) != -1) {
		if((occ = strstr(line, SQL_TAG)) != NULL) {
			query = strstr(occ, "query") + 7;
			query[strlen(query) - 3] = '\0';
			generateHTMLFromQuery(query, tmpFile);
		} else {
			fprintf(tmpFile, line);
		}
	}
	
	fclose(orgFile);
	fclose(tmpFile);
	free(line);
	
	return tmpFileName;
}

void* thread(void* arg) {

	struct thread_params* params = (struct thread_params*) arg;
	ServerTCP* serverSocket = params->serverSocket;
	ServerConnection* connection = params->connection;

	char* rqst = connection->recv();
	char* pageName = getFileNameFromRqst(rqst);
	char* pagePath = getFilePath(pageName);
	char* tmpFilePath = NULL;
	
	printf("[I] Connection established with host -> %s\n[I] Request Header :\n%s\n",/*connection->getConnectedAddress().toString(),*/ pageName, rqst);
	
	if(access(pagePath, R_OK) != -1) {
		tmpFilePath = gdi(pagePath);
    	connection->sendRaw((char*)HEADER_200, strlen((char*)HEADER_200));
    	connection->sendFile((char*)tmpFilePath);
		printf("[I] Sent %s page\n", pagePath);
	} else {
    	connection->sendRaw((char*)HEADER_404, strlen((char*)HEADER_404));
		connection->sendFile((char*)"html/404.html");
		printf("[W] %s Not Found : 404 ERROR %s %s\n", pageName, pagePath, strerror(errno));
	}

	free(pageName);
	free(pagePath);
	free(tmpFilePath);
	free(rqst);

	serverSocket->closeConnection(connection);

	pthread_exit(0);
}

char* getFileNameFromRqst(char* rqst) {
	
	regex_t regex;
	regmatch_t groupArray[MAX_GROUPS];
	char* fileName = NULL;
	
	int ret = regcomp(&regex, REGEX, REG_EXTENDED);
	if(!ret) {
		ret = regexec(&regex, rqst, MAX_GROUPS, groupArray, 0);
		if(!ret) {
		    char sourceCopy[(strlen(rqst) + 1) * sizeof(char)];
		    strcpy(sourceCopy, rqst);
		    sourceCopy[groupArray[1].rm_eo] = '\0';
		   
		    if(strcmp(sourceCopy + groupArray[1].rm_so, "/") != 0)
		    	fileName = strdup(sourceCopy + groupArray[1].rm_so);
		    else
		    	fileName = strdup(DEFAULT_PAGE);	
		}
	}
	
	regfree(&regex);
	return fileName;
}

char* getFilePath(char* pageName) {
	return concatStr((char*)DEFAULT_HTML_DIR, pageName);
}
