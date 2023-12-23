#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <inttypes.h>
#include <ctype.h>
#include <limits.h>

#define BUFSIZE 1024*50

//Common.c
void get_lines(char* fname, void (*line_handler)(char*, int, void*), void* params);
int count_chars(const char* str, char c);
int intersection_size(int* arr1, int* arr2, int n, int m);
void print_arr(int* arr, int n);
int sum_arr(int* arr, int n);
void init_arr(int* arr, int value, int n);
void get_numbers_ll(char* str, long long* dest, int* num);
void get_numbers_int(char* str, int* dest, int* num);
int num_unique(char* str, int len);
int max_occ_nc(char* str, int len, char c);
char most_occ_nc(char* str, int len, char c);
void forward_diff(int* arr, int* diff, int n);
int nnz(int* arr, int n);
void reverse_arr(int* arr, int* rev, int n);
int sign(int x);
int max(int a, int b);
int min(int a, int b);
int overlap(int min1, int max1, int min2, int max2);
int element_in_arr(int* arr, int ele, int n);

//Ported to asm
int32_t adjacent(int32_t, int32_t, int32_t);
int32_t between(int32_t i, int32_t j, int32_t k);
int64_t concat(int64_t x, int64_t y);
int64_t LCM(int64_t a, int64_t b);
int64_t LCMM(int64_t* arr, int64_t n);
