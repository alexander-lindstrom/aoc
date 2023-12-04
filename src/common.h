#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define BUFSIZE 256
void get_lines(char* fname, void (*line_handler)(char*, int, void*), void* params);
int count_chars(const char* str, char c);
int adjacent(int i, int j, int dim);
int intersection_size(int* arr1, int* arr2, int n, int m);
void print_arr(int* arr, int n);
int sum_arr(int*, int n);
void init_arr(int*, int value, int n);