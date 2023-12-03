#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define BUFSIZE 256
void get_lines(char* fname, void (*line_handler)(char*, int, void*), void* params);
int count_chars(const char* str, char c);
int adjacent(int i, int j, int dim);