#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFSIZE 256
void get_lines(char* fname, void (*line_handler)(char*, void*), void* params);
int count_chars(const char* str, char c);