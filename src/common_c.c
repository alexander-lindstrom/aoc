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