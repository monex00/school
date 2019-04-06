#include <stdio.h>
#include <regex.h>
#include <stdlib.h>
#include <string.h>

#define STR "GET /pippo.html HTTP/1.1"
#define REGEX "^GET\\s([^\\?\\*<>|]+)\\sHTTP"
#define MAX_GROUPS 2

regex_t regex;
int ret;
char msgbuf[100];
regmatch_t groupArray[MAX_GROUPS];

int main(int argc, char* argv[]) {
	ret = regcomp(&regex, REGEX, REG_EXTENDED);
	if(ret) {
		printf("Error regcomp\n");
		exit(1);	
	}
	ret = regexec(&regex, STR, MAX_GROUPS, groupArray, 0);
	if(!ret) {
		printf("Match! : ");

         char sourceCopy[strlen(STR) + 1];
         strcpy(sourceCopy, STR);
         sourceCopy[groupArray[1].rm_eo] = '\0';
         printf("%s\n", sourceCopy + groupArray[1].rm_so);

	} else if(ret == REG_NOMATCH) {
		printf("No Match :(\n");
	} else {
		regerror(ret, &regex, msgbuf, sizeof(msgbuf));
		fprintf(stderr, "Regex match failed: %s\n", msgbuf);
		exit(1);
	}	

	regfree(&regex);
}
