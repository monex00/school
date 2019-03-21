#include "libs/ServerTCP.hpp"
#include <sqlite3.h>
#include <stdio.h>
#include "../libs/UtilExt.h"

#define DB_PATH "db"

#define MAX_LINE_LEN 1000
#define TMP_FILE_NAME_LEN 16

#define DEFAULT_PAGE "index.html"

#define SQL_TAG "<SQL"

#define DEFAULT_HTML_DIR "html/"
#define DEFAULT_HTML_TMP_DIR "html/tmp/"
#define HTML_EXT ".html"

#define HEADER_200 "HTTP/1.1 200 OK\n\n"
#define HEADER_404 "HTTP/1.1 404 File Not Found\n\n"

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

	if(argc != 2) {
		printf("USAGE: %s PORT\n", argv[0]);
		return -1;
	}
	
	int port = atoi(argv[1]);

	Address mySelf(IP_LOOPBACK, port);
	ServerTCP serverSocket(mySelf);

	while(true) {
		ServerConnection* connection = serverSocket.accept();
		struct thread_params params;
		params.serverSocket = &serverSocket;
		params.connection = connection;
		pthread_t thid;
		pthread_create(&thid, NULL, thread, (void*)&params);
		//pthread_join(thid, NULL);	
	}

}

void generateHTMLFromQuery(char* query, FILE* fp) {
	sqlite3 *db;
	sqlite3_stmt* res;
	char* err_msg;
	
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
    
    free(toWrite);
    return 0;
}

char* gdi(char* filePath) {	
	FILE* orgFile;
	FILE* tmpFile;
	char* line;
	char* occ;
	char* query;
	char* tmpFileName;
	size_t len;
	ssize_t resRead;
	
	line = (char*) malloc(MAX_LINE_LEN * sizeof(char));
	
	tmpFileName = (char*)malloc(TMP_FILE_NAME_LEN * sizeof(char));
	
	orgFile = fopen(filePath, "r");
	sprintf(tmpFileName, "%s%d%s", DEFAULT_HTML_TMP_DIR, pthread_self(), HTML_EXT);
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
	
	if(line)free(line);
	
	return tmpFileName;
}

void* thread(void* arg) {

	struct thread_params* params = (struct thread_params*) arg;
	ServerTCP* serverSocket = params->serverSocket;
	ServerConnection* connection = params->connection;

	char* rqst = connection->recv();
	char* pageName = getFileNameFromRqst(rqst);
	char* pagePath = getFilePath(pageName);
	char* tmpFilePath;
	
	printf("[I] Connection established with host : %s -> %s\n[I] Request Header :\n%s\n", connection->getConnectedAddress().toString(), pageName, rqst);
	
	if(access(pagePath, F_OK) != -1 ) {
		tmpFilePath = gdi(pagePath);
    	connection->sendRaw((char*)HEADER_200, strlen((char*)HEADER_200));
		connection->sendFile((char*)tmpFilePath);
		printf("[I] Sent %s page\n", pagePath);
	} else {
    	connection->sendRaw((char*)HEADER_404, strlen((char*)HEADER_404));
		connection->sendFile((char*)"html/404.html");
		printf("[W] %s Not Found : 404 ERROR %s\n", pageName, pagePath);
	}

	free(pageName);
	
	//free(pagePath); ERR
	
	//free(tmpFilePath); ERR
	
	free(rqst);

	serverSocket->closeConnection(connection);

	pthread_exit(0);
}

char* getFileNameFromRqst(char* rqst) {
	char* tmpRqst = strdup(rqst);
	char* splittedString = strtok(tmpRqst, "/");
	splittedString = strtok(NULL, " ");
	
	char* pageName;
	pageName = strdup(splittedString);
	
	free(tmpRqst);
	
	return pageName;
}

char* getFilePath(char* pageName) {
	return concatStr((char*)DEFAULT_HTML_DIR, pageName);
}
