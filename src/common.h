#include <stdio.h>
#include <stdlib.h>

#define BUFSIZE 256
void get_lines(char* fname, void (*line_handler)(char*, void*), void* params);