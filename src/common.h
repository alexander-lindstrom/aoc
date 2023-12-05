#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <inttypes.h>
#include <ctype.h>
#include <limits.h>

#define BUFSIZE 256

//Common.c
void get_lines(char* fname, void (*line_handler)(char*, int, void*), void* params);
int count_chars(const char* str, char c);
int intersection_size(int* arr1, int* arr2, int n, int m);
void print_arr(int* arr, int n);
int sum_arr(int*, int n);
void init_arr(int*, int value, int n);
void get_numbers(char* str, long long* dest, int* num);

//Ported to asm
int64_t adjacent(int64_t, int64_t, int64_t);
int64_t between(int64_t i, int64_t j, int64_t k);
