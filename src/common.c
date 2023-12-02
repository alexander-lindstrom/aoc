#include "common.h"

void get_lines(char* fname, void (*line_handler)(char*, void*), void* params){

	char buffer[BUFSIZE]; 
	FILE* fp = fopen(fname, "r");

	if (!fp){
		printf("Couldn't open file: %s\n", fname);
		exit(-1);
	}

	while(fgets(buffer, BUFSIZE, fp)){
		line_handler(buffer, params);
	}
}

//Assumes newline at the end of string
int count_chars(const char* str, char c){

	int i = 0, count = 0;
	
	while(1){
		if (str[i] == '\n' || str[i] == 0){
			break;
		}
		if(str[i] == c){
			count++;
		}
		i++;
	}
	return count;
}