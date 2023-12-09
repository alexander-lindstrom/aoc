#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <inttypes.h>
#include <ctype.h>
#include <limits.h>

#define BUFSIZE 1024

//Common.c
void get_lines(char* fname, void (*line_handler)(char*, int, void*), void* params);
int count_chars(const char* str, char c);
int intersection_size(int* arr1, int* arr2, int n, int m);
void print_arr(int* arr, int n);
int sum_arr(int*, int n);
void init_arr(int*, int value, int n);
void get_numbers_ll(char* str, long long* dest, int* num);
void get_numbers_int(char* str, int* dest, int* num);
int num_unique(char* str, int len);
int max_occ_nc(char* str, int len, char c);
char most_occ_nc(char* str, int len, char c);
long long LCMM(int* arr, int n);
void forward_diff(int* arr, int* diff, int n);
int nnz(int* arr, int n);
void reverse_arr(int* arr, int* rev, int n);


//Ported to asm
int64_t adjacent(int64_t, int64_t, int64_t);
int64_t between(int64_t i, int64_t j, int64_t k);
int64_t concat(int64_t x, int64_t y);
int64_t LCM(int64_t a, int64_t b);
