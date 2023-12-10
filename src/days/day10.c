#include "../common.h"

#define DIM 140
#define MAXSTEPS 20000
typedef struct Element{
	int n1;
	int n2;
	char label;
}Element;

typedef struct State{
  Element elements[DIM*DIM];
	int start;
	int nsteps;
	int history[MAXSTEPS];
}State;

void handle_line(char* line, int row, void* params){
	
  State* s = (State*) params;
	int i = 0;
	while(line[i] != 0 && line[i] != '\n'){
		
		int index = row*DIM + i;
		//Neighbors can end up outside array range.
		s->elements[index].label = line[i]; 
		switch(line[i]){
			case '|':
				s->elements[index].n1 = index - DIM;
				s->elements[index].n2 = index + DIM;
				break;
			case '-':
				s->elements[index].n1 = index - 1;
				s->elements[index].n2 = index + 1;
				break;
			case 'L':
				s->elements[index].n1 = index - DIM;
				s->elements[index].n2 = index + 1;
				break;
			case 'J':
				s->elements[index].n1 = index - DIM;
				s->elements[index].n2 = index - 1;
				break;
			case '7':
				s->elements[index].n1 = index - 1;
				s->elements[index].n2 = index + DIM;
				break;
			case 'F':
				s->elements[index].n1 = index + 1 ;
				s->elements[index].n2 = index + DIM;
				break;
			case '.':
				break;
			case 'S':
				s->start = index;
				break;
			default:
				printf("Invalid state!\n");
				exit(-1);
		}
		i++;
	}
}

int loop_area(State s){
	
	int A = 0;
	for(int i = 1; i < s.nsteps; i++){
		int x1 = s.history[i-1] % DIM, y1 = s.history[i-1] / DIM;
		int x2 = s.history[i] % DIM, y2 = s.history[i] / DIM;
		A += x1*y2 - y1*x2;
	}
	return A/2;
} 

int get_next(State s, int cur, int prev){
	if(s.elements[cur].n1 != prev){
		return s.elements[cur].n1;
	}
	return s.elements[cur].n2;
}

// j is connected to i
int path_exists(State s, int i, int j){
	if(s.elements[j].n1 == i){
		return 1;
	}
	return s.elements[j].n2 == i;
}

int loop(State* s, int start, int cur){
	
	if(!between(cur, 0, DIM*DIM) || s->elements[cur].label == '.' || !path_exists(*s, start, cur)){
		return 0;
	}
	
	s->history[0] = start;
	s->nsteps = 1;	// Compensate for starting as S neighbor
	int prev = start;
	while(cur != start){
		if(!between(cur, 0, DIM*DIM)){
			return 0;
		}
		
		s->history[s->nsteps] = cur;
		int next = get_next(*s, cur, prev);
		prev = cur;
		cur = next;
		s->nsteps++;
		
	}
	// Since the last step is never taken
	s->history[s->nsteps] = cur;
	s->nsteps++;
	return s->nsteps / 2;
}

//Part 2: https://en.wikipedia.org/wiki/Pick%27s_theorem
//I = A - b/2 + 1 where A is the polygon area and b is the length of the loop
//And for A: https://en.wikipedia.org/wiki/Shoelace_formula
int score(State s, int part){
	
	//N, E, S, W
	int directions[] = {-DIM, 1, DIM, -1};
	for(int i = 0; i < 4; i++){
		int res = loop(&s, s.start, s.start + directions[i]);
		if(res && part == 1){
			return res;
		}
		else {
			return loop_area(s) - res + 1;
		}
	}
	printf("No loops found!\n");
	exit(-1);
}

int main(int argc, char *argv[]){
  
  char* fname = "data/day10.txt";
  State s = {.start = 0, .nsteps = 0};
  get_lines(fname, handle_line, &s);
	printf("Score1: %d\n", score(s, 1));
	printf("Score2: %d\n", score(s, 2));
  return 0;
}