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

void get_numbers_ll(char* str, long long* dest, int* num){
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

void get_numbers_int(char* str, int* dest, int* num){
  char *p = str;
  while (*p) {
    if (isdigit(*p) || ((*p=='-'||*p=='+') && isdigit(*(p+1)))){
      dest[*num] = strtol(p, &p, 10);
      (*num)++;
    } else {
      p++;
    }
  }
}

int max_occ_nc(char* str, int len, char c){

	int max = 0;
	for(int i = 0; i < len; i++){
		int cur = 0;
		for(int j = 0; j < len; j++){
			if (str[i] == str[j] && str[i] != c){
				cur++;
				if (cur > max){
					max = cur;
				}
			}
		}
	}
	return max;
}

char most_occ_nc(char* str, int len, char c){

	int max = 0;
	char most = 0;
	for(int i = 0; i < len; i++){
		int cur = 0;
		for(int j = 0; j < len; j++){
			if (str[i] == str[j] && str[i] != c){
				cur++;
				if (cur > max){
					max = cur;
					most = str[i];
				}
			}
		}
	}
	return most;
}

int num_unique(char* str, int len){
	int num_unique = 0;
	for(int i = 0; i < len; i++){
		int flag = 0;
		for(int j = 0; j < i; j++){
			if (str[i] == str[j]){
				flag = 1;
				break;
			}
		}
		if (flag == 0){
			num_unique++;
		}
	}
	return num_unique;
}

long long gcd(long long a, long long b) {

	int remainder = a%b;
	if(remainder == 0){
		return b;
	}
	return gcd(b, remainder);
}

long long LCM(long long a, long long b){
	long long res = a*b/gcd(a, b);;
	printf("a: %lld, b: %lld, res: %lld\n", a, b, res);
	return res;
}

// note that lcm(a,b,c) = lcm(a,lcm(b,c))
long long LCMM(int* arr, int n){
	
	print_arr(arr, n);
	
	if (n < 2){
		printf("2 or more elements required\n");
		exit(-1);
	}
	
	long long current = LCM(arr[0], arr[1]);
	for(int i = 2; i < n; i++){
		current = LCM(current, arr[i]);
	}
	return current;
}

void forward_diff(int* arr, int* diff, int n){
	
	for(int i = 0; i < n-1; i++){
		diff[i] = arr[i+1] - arr[i];
	}
}

int nnz(int* arr, int n){
	int count = 0;
	for(int i = 0; i < n; i++){
		if (arr[i]!= 0){
			count++;
		}
	}
	return count;
}

void reverse_arr(int* arr, int* rev, int n){
	
	for(int i = 0; i < n; i++){
		rev[i] = arr[n-i-1];
	}
}