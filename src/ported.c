#include "common.h"

//8 neighbors
int32_t adjacent(int32_t i, int32_t j, int32_t dim){
	
	int icol = i%dim, jcol = j%dim;
	int irow = i/dim, jrow = j/dim;
	
	if (abs(irow - jrow) > 1){
		return 0;
	}
	return (abs(icol - jcol) < 2);
}

int32_t between(int32_t i, int32_t j, int32_t k){
  return i >= j && i <=k;
}

int64_t concat(int64_t x, int64_t y){
  int64_t pow = 10;
  while(y >= pow){
		pow *= 10;
	}
  return x * pow + y;        
}

int32_t gcd(int32_t a, int32_t b) {

	int remainder = a%b;
	if(remainder == 0){
		return b;
	}
	return gcd(b, remainder);
}

int64_t LCM(int64_t a, int64_t b){
	int64_t res = a*b/gcd(a, b);;
	return res;
}

// note that lcm(a,b,c) = lcm(a,lcm(b,c))
int64_t LCMM(int64_t* arr, int64_t n){
	
	if (n < 2){
		printf("2 or more elements required\n");
		exit(-1);
	}
	
	int64_t current = LCM(arr[0], arr[1]);
	for(int i = 2; i < n; i++){
		current = LCM(current, arr[i]);
	}
	return current;
}