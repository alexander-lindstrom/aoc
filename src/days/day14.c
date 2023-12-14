#include "../common.h"

#define DIM 100
#define NORTH -DIM
#define EAST 1
#define SOUTH DIM
#define WEST -1
#define DIRS 4
#define ITER 1000
enum types{
  EMPTY,
  ROCK,
  STATIC_ROCK
};

typedef struct State{
	int rocks[DIM*DIM];
}State;

int directions[DIRS] = {NORTH, WEST, SOUTH, EAST};

void handle_line(char* line, int row, void* params){
	
  State* s = (State*) params;
	int i = 0;
	while(line[i] != 0 && line[i] != '\n'){
		
		switch(line[i]){
			case '#':
				s->rocks[row*DIM + i] = STATIC_ROCK;
				break;
			case 'O':
				s->rocks[row*DIM + i] = ROCK;
				break;
			case '.':
				s->rocks[row*DIM + i] = EMPTY;
				break;
		}
		i++;
	}
}

void tilt(State* s, int dir){
	
	int changed = 1;
	while(changed){
		changed  = 0;
		
		switch(dir){
			//Iterating forward is reasonable for north, west
			case NORTH:
			case WEST:
			for(int i = 0; i < DIM*DIM; i++){
				//Don't loop around
				if(dir ==  WEST){
					if((i/DIM) != (i+dir)/DIM){
						continue;
					}
				}
				if (s->rocks[i] == ROCK && between(i + dir, 0, DIM*DIM) && s->rocks[i + dir] == EMPTY){
					s->rocks[i] = EMPTY;
					s->rocks[i + dir] = ROCK; 
					changed++;
				}
			}
			break;
			//Iterating backwards is reasonable for south, east
			case SOUTH:
			case EAST:
			for(int i = DIM*DIM-1; i >= 0; i--){
				//Don't loop around
				if(dir ==  EAST){
					if((i/DIM) != (i+dir)/DIM){
						continue;
					}
				}
				if (s->rocks[i] == ROCK && between(i + dir, 0, DIM*DIM) && s->rocks[i + dir] == EMPTY){
					s->rocks[i] = EMPTY;
					s->rocks[i + dir] = ROCK; 
					changed++;
				}
			}
			break;
		}
	}
}

int load(State s){
	int sum = 0;
	for(int i = 0; i < DIM*DIM; i++){
		if(s.rocks[i] == ROCK){
			int row = i/DIM;
			sum += DIM - row;
		}
	}
	return sum;
}

int score1(State s){
	tilt(&s, NORTH);
	return load(s);
}

void get_cycle(int* arr, int* len, int* start){
	
	*start = 0;
	*len = 0;
	int value = 0;
	
	//Get starting point - let sequence settle first
	for(int i = ITER/10; i < ITER; i++){
		
		for(int j = 0; j < i; j++){
			if(arr[j] == arr[i]){
				*start = i;
				value = arr[i];
				goto done;
			}
		}
	}
	done:
	
	//Get length
	for(int i = *start + 1; i < ITER; i++){
		if(arr[i] == value){
			*len = i-*start;
			break;
		}
	}
}

int score2(State s){
	
	int res[ITER];
	for(int i = 0; i < ITER; i++){
		for(int i = 0; i < DIRS; i++){
			tilt(&s, directions[i]);
		}
		res[i] = load(s);
	}
	int cycle_length, cycle_start;
	print_seq(res, ITER);
	get_cycle(res, &cycle_length, &cycle_start);
	int remainder = (1000000000 - (cycle_start+1)) % cycle_length;
	return res[cycle_start + remainder];
	
}

int main(int argc, char *argv[]){
  
  char* fname = "data/day14.txt";
  State s = {0};
  get_lines(fname, handle_line, &s);
	printf("Part1: %d\n", score1(s));
	printf("Part2: %d\n", score2(s));
  return 0;
}