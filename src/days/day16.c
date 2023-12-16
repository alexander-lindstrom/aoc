#include "../common.h"

#define DIM 110
//#define DIM 10
#define NORTH -DIM
#define EAST 1
#define SOUTH DIM
#define WEST -1
#define DIRS 4
#define MAXMAP DIM*DIM
#define VERTICAL_BRANCH 2
#define HORIZONTAL_BRANCH 3

typedef struct Key{
	int pos;
	int dir[DIRS];
}Key;

typedef struct State{
	char grid[DIM*DIM];
	Key map[MAXMAP];
}State;

int directions[DIRS] = {NORTH, EAST, SOUTH, WEST};

void trace(State* s, int cur, int dir);

void handle_line(char* line, int row, void* params){
	
  State* s = (State*) params;
	int i = 0;
	while(line[i] != 0 && line[i] != '\n'){
		s->grid[row*DIM + i] = line[i];
		i++;
	}
}

int key_exist(State* s, int pos, int dir){
	for(int i = 0; i < DIRS; i++){
		if(s->map[pos].dir[i] == dir){
			return 1;
		}
	}
	return 0;
}

void add_key(State* s, int pos, int dir){
	if(key_exist(s, pos, dir)){
		return;
	}
	for(int i = 0; i < DIRS; i++){
		if(s->map[pos].dir[i] == 0){
			s->map[pos].dir[i] = dir;
			return;
		}
	}
}

void reset_map(State* s){
	for(int i = 0; i < MAXMAP; i++){
		for(int j = 0; j < DIRS; j++){
			s->map[i].dir[j] = 0;
		}
	}
}

int direction(State* s, int cur, int dir){
	switch(s->grid[cur]){
		case '/':
			return abs(dir) == DIM ? -sign(dir) : -sign(dir) * DIM;
		
		case '\\':
			return abs(dir) == DIM ? sign(dir) : sign(dir) * DIM;
		
		case '-':
			if(dir == NORTH || dir == SOUTH){
				return HORIZONTAL_BRANCH;
			}
			break;

		case '|':
			if(dir == EAST || dir == WEST){
				return VERTICAL_BRANCH;
			}
			break;
	}
	return dir;
}

int branch(State* s, int cur, int dir){
	
	if(dir == HORIZONTAL_BRANCH){
		trace(s, cur, EAST);
		return WEST;
		
	}
	else if(dir == VERTICAL_BRANCH){
		trace(s, cur, SOUTH);
		return NORTH;
	}
	return dir;
}

void trace(State* s, int cur, int dir){
	
	dir = direction(s, cur, dir);
	dir = branch(s, cur, dir);
	
	while(1){
		
		if(key_exist(s, cur, dir)){
			return;
		}
		add_key(s, cur, dir);
		int next = cur + dir;
		if(!between(next, 0, DIM*DIM-1)){
			return;
		}
		//Don't loop around
		if(dir == EAST || dir == WEST){
			if(next/DIM != cur/DIM){
				return;
			}
		}
		cur = next;
		dir = direction(s, cur, dir);
		dir = branch(s, cur, dir);
	}
}

int energized(State* s){
	int sum = 0;
	for(int i = 0; i < DIM; i++){
		for(int j = 0; j < DIM; j++){
			for(int k = 0; k < DIRS; k++){
				if(s->map[i*DIM + j].dir[k]){
					sum++;
					break;
				}
			}
		}
	}
	return sum;
}

int score1(State* s){
	trace(s, 0, EAST);
	return energized(s);
}

int iterate(State* s, int dir){
	
	int max = 0;
	if(dir == NORTH || SOUTH){
		int i = dir == SOUTH ? DIM - 1 : 0;
		for(int j = 0; j < DIM; j++){
			trace(s, i*DIM + j, NORTH);
			int score = energized(s);
			if(score > max){
				max = score;
			}
			reset_map(s);
		}
	}
	else if(dir == EAST || WEST){
		int j = dir == EAST ? DIM - 1 : 0;
		for(int i = 0; i < DIM; i++){
			trace(s, i*DIM + j, NORTH);
			int score = energized(s);
			if(score > max){
				max = score;
			}
			reset_map(s);
		}
	}
	return max;
}

int score2(State* s){
	
	int max = 0;
	for(int i = 0; i < DIRS; i++){
		int res = iterate(s, directions[i]);
		if(res > max){
			max = res;
		}
	}
	return max;
}

int main(int argc, char *argv[]){
  
  char* fname = "data/day16.txt";
  State s = {0};
  get_lines(fname, handle_line, &s);
	printf("Part1: %d\n", score1(&s));
	printf("Part2: %d\n", score2(&s));
  return 0;
}