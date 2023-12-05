#include "common.h"


void get_lines(char* fname, void (*line_handler)(char*, int, void*), 
	void* params){

	char buffer[BUFSIZE]; 
	FILE* fp = fopen(fname, "r");

	if (!fp){
		printf("Couldn't open file: %s\n", fname);
		exit(-1);
	}
	
	int i = 0;
	while(fgets(buffer, BUFSIZE, fp)){
		line_handler(buffer, i, params);
		i++;
	}
}

//Newline will terminate
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

//8 neighbors
int adjacent_c(int i, int j, int dim){
	
	int icol = i%dim, jcol = j%dim;
	int irow = i/dim, jrow = j/dim;
	
	if (abs(irow - jrow) > 1){
		return 0;
	}
	return (abs(icol - jcol) < 2);
}

//Get the size of the intersection of arr1, arr2
int intersection_size(int* arr1, int* arr2, int n, int m){
  
  int size = 0;
  for(int i = 0; i < n; i++){
    for(int j = 0; j < m; j++){
      if (arr1[i] == arr2[j]){
        size++;
      }
    }
  }
  return size;
}

void print_arr(int* arr, int n){
  
  for(int i = 0; i < n; i++){
    printf("%d ", arr[i]);
  }
  printf("\n");
}

int sum_arr(int* arr, int n){
  
  int sum = 0;
  for(int i = 0; i < n; i++){
    sum += arr[i];
  }
  return sum;
}

void init_arr(int* arr, int value, int n){
  
  for(int i = 0; i < n; i++){
    arr[i] = value;
  }
  printf("\n");
}

// fill dest, increment num
void get_numbers(char* str, long long* dest, int* num){
  char *p = str;
  while (*p) {
    if (isdigit(*p) || ((*p=='-'||*p=='+') && isdigit(*(p+1)))){
      dest[*num] = strtoll(p, &p, 10);
      (*num)++;
    } else {
      p++;
    }
  }
}

int between(long long i, long long j, long long k){
  return i >= j && i <=k;
}