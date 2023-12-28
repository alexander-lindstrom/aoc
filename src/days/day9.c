#include "../common.h"

#define MAXROWS 300
#define MAXCOLS 50

typedef struct Row{
	int ncols;
	int cols[MAXCOLS];
}Row;

typedef struct State{
	int nrows;
  Row rows[MAXROWS];
}State;

void handle_line(char* line, int row, void* params){

  State* s = (State*) params;
	get_numbers_int(line, s->rows[s->nrows].cols, &s->rows[s->nrows].ncols);
	s->nrows++;
	
}

int extrapolate(int* arr, int n){

	int diff[n-1];
	forward_diff(arr, diff, n);
	
	if(nnz(diff, n-1) == 0){
		return arr[n-1];
	}
	return arr[n-1] + extrapolate(diff, n-1);
}

long long score(State s, int part){
	
	long long sum = 0;
	for(int i = 0; i < s.nrows; i++){
		if(part == 2){
			int rev[s.rows[i].ncols];
			reverse_arr(s.rows[i].cols, rev, s.rows[i].ncols);
			sum += extrapolate(rev, s.rows[i].ncols);
		}
		else{
			sum += extrapolate(s.rows[i].cols, s.rows[i].ncols);
		}
	}
	return sum;
}

int main(int argc, char *argv[]){

	char* fname = "data/day9.txt";
	State s = {.nrows = 0};
  get_lines(fname, handle_line, &s);
	printf("Part1: %lld\n", score(s, 1));
	printf("Part2: %lld\n", score(s, 2));
  return 0;
}