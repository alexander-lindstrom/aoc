#include "../common.h"

#define DIM 140
typedef struct State{
	int ngalaxies;
	int rowc[DIM*DIM];
	int colc[DIM*DIM];
	int zero_row[DIM];
	int zero_col[DIM];
}State;

void handle_line(char* line, int row, void* params){
	
  State* s = (State*) params;
	int i = 0, flag = 0;
	while(line[i] != 0 && line[i] != '\n'){
		
		int col = i;
		if (line[i] == '#'){
			s->rowc[s->ngalaxies] = row;
			s->colc[s->ngalaxies] = col;
			flag = 1;
			s->ngalaxies++;
		}
		i++;
	}
	if (!flag){
		s->zero_row[row] = 1;
	}
}

void update_row(State* s, int C){
	
	int sum = 0;
	int displacement[DIM];
	init_arr(displacement, 0, DIM);
	for(int i = 0; i < DIM; i++){
		displacement[i] = sum;
		if(s->zero_row[i]){
			sum += C;
		}
	}
	
	for(int i = 0; i < s->ngalaxies; i++){
		s->rowc[i] += displacement[s->rowc[i]];
	}
}

void update_col(State* s, int C){
	
	//Set zero_col
	for(int i = 0; i < DIM; i++){
		int count = 0;
		for(int j = 0; j < s->ngalaxies; j++){
			if(i == s->colc[j]){
				count++;
				break;
			}
		}
		if(!count){
			s->zero_col[i] = 1;
		}
	}
	
	//Update
	int sum = 0;
	int displacement[DIM];
	init_arr(displacement, 0, DIM);
	for(int i = 0; i < DIM; i++){
		displacement[i] = sum;
		if(s->zero_col[i]){
			sum += C;
		}
	}
	for(int i = 0; i < s->ngalaxies; i++){
		s->colc[i] += displacement[s->colc[i]];
	}
}

void expand_universe(State* s, int C){
	update_row(s, C);
	update_col(s, C);
}

long long taxicab_dist(int row1, int col1, int row2, int col2){
	return abs(col1 - col2) + abs(row1 - row2);
}

long long score(State s){

	long long sum = 0;
	int count = 0;
	for(int i = 0; i < s.ngalaxies; i++){
		for(int j = i+1; j < s.ngalaxies; j++){
			sum += taxicab_dist(s.rowc[i], s.colc[i], s.rowc[j], s.colc[j]);
		}
	}
	return sum;
}

int main(int argc, char *argv[]){
  
	int C = 1;
	if(argc != 2){
		printf("Example: day part1");
	}
	if(strcmp(argv[1], "part2") == 0){
		C = 1e6-1;
	}
  char* fname = "data/day11.txt";
  State s = {0};
  get_lines(fname, handle_line, &s);
	expand_universe(&s, C);
	printf("Score: %lld\n", score(s));
  return 0;
}