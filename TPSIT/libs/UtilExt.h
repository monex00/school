#ifndef __UTIL_EXT_H
#define __UTIL_EXT_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

void error(const char* error_proc_msg, int error_proc_num);
char** split(char* s, char c);
char* find(char* s, char c);
char* dupStr(char* s);
int lenStr(char* s);
char** dupVett(char** v);
int lenVett(char** v);
char* inputStr();
int cmpStr(char* s1, char* s2);

void error(const char* error_proc_msg, int error_proc_num) {
    printf("\n\n!* [MAIN PROC] %s, %d\n  %s, %d", error_proc_msg, error_proc_num, strerror(errno), errno);
    exit(error_proc_num);
}

char** split(char* s, char c) {
	char** ret;
	char* buffer[4097];
	char* pos;
	int i = 0;
	
	while(pos = find(s,c)) {
		*pos = '\0';
		buffer[i++] = dupStr(s);
		s = pos + 1;
	}
	
	buffer[i++] = dupStr(s);
	buffer[i] = NULL;
	
	ret = dupVett(buffer);
	
	return ret;
}

char* find(char* s, char c) {
	for(; *s; s++)
		if(*s == c) return s;
	return NULL;
}

char* dupStr(char* s) {
	char* ret;
	int i;
	
	ret = (char *)malloc((lenStr(s) + 1) * sizeof(char));
	
	for(i = 0; *(ret + i) = *(s + i); i++) {}
	
	return ret;
}

int lenStr(char* s) {
	int ret;
	
	for(ret = 0; *(s + ret); ret++) {}
	
	return ret;
}

char** dupVett(char** v){
	char** ret;
	int i;
	
	ret = (char**)malloc((lenVett(v) + 1) * sizeof(char*));
	for(i = 0; *(ret + i) = *(v + i); i++) {}
	
	return ret;
}

int lenVett(char** v) {
	int ret;
	
	for(ret = 0; *(v + ret); ret++){}
	
	return ret;
}

char* inputStr() {
	char buffer[4097];
	int i = 0;
	char c = getchar();
	while(c != '\n') {
		buffer[i] = c;
		c = getchar();
		i++;
	}
	
	buffer[i] = '\0';
	return dupStr(buffer);
}

int cmpStr(char* s1, char* s2) {
	if(*s1 > *s2) return -1;
	if(*s1 < *s2) return 1;
	if(*s1 != '\0') return cmpStr(++s1, ++s2);
	return 0;
}

char* concatStr(char* s1, char* s2) {

	char* ret;
	int i, j;
	
	ret = (char*)malloc((lenStr(s1) + lenStr(s2) + 1) * sizeof(char));
	
	for(i = 0; *(ret + i) = *(s1 + i); i++) {}
	for(j = 0; *(ret + j + i) = *(s2 + j); j++) {}
	
	return ret;

}

char* deleteChar(char* s, char c) {
	char* ret;
	char* p;
	int i;
	
	ret = dupStr(s);
	p = find(ret, c);
	
	for(i = 0; *(p + i) = *(p + i + 1); i++) {}
	
	return ret;
}

char* addChar(char* s, char c) {
	char* ret;
	int i;
	
	ret = (char*)malloc((lenStr(s) + 1) * sizeof(char));
	
	for(i = 0; *(ret + i) = *(s + i); i++) {}
	*(ret + i++) = c;
	*(ret + i) = '\0';
	
	return ret;
}

#endif //__UTIL_EXT_H
