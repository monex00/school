#define REGEX "<SQL\\s+db=\"(.+)\"\\s+query=\"(.+);\"\\s*\\/>"
#define REGEX2 "^GET\\s([^\\?\\*<>|]+)\\sHTTP"
#define TEXT_TO_MATCH "<SQL db=\"./a/a.db\" query=\"SELECT * FROM a WHERE a = 5;\"/>"
#define TEXT_TO_MATCH_2 "GET /wiki/Pagina_principale HTTP/1.1"

#include <stdio.h>
#include <regex.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

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

int main(int argc, char* argv[]) {
	char** res = (char**)malloc(2 * sizeof(char*));
	int ret = regexGroupExtractor((char*)REGEX, (char*)TEXT_TO_MATCH, res, 2);
	if(ret == 0)
		for(int i = 0; i < 2; i++)
			printf("Group num [%d] : %s\n", i + 1, res[i]);
	else
		printf("ERROR\n");

	return 0;
}
