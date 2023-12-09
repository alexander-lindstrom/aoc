#include "common.h"

//8 neighbors
int64_t adjacent(int64_t i, int64_t j, int64_t dim){
	
	int icol = i%dim, jcol = j%dim;
	int irow = i/dim, jrow = j/dim;
	
	if (abs(irow - jrow) > 1){
		return 0;
	}
	return (abs(icol - jcol) < 2);
}

int64_t between(int64_t i, int64_t j, int64_t k){
  return i >= j && i <=k;
}

int64_t concat(int64_t x, int64_t y){
  int64_t pow = 10;
  while(y >= pow){
		pow *= 10;
	}
  return x * pow + y;        
}

int64_t gcd(int64_t a, int64_t b) {

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