#include "../common.h"

#define MAXITEMS 5
#define LEN 3
#define MAXNODES 1000
#define MAXINSTR 1000

typedef struct Node{
	int left;
	int right;
	char str_left[LEN + 1];
	char str_right[LEN + 1];
}Node;

typedef struct State{
  int num_nodes;
	Node nodes[MAXNODES] ;
	char lookup[MAXNODES][LEN + 1];
	int num_instructions;
	int instructions[MAXINSTR];
}State;

void handle_line(char* line, int row, void* params){
	
  State* s = (State*) params;
  char substr[MAXINSTR];
	memset(substr, 0, MAXINSTR);
	int i = 0, j = 0, found = 0;
	
	while(line[i] != 0 && line[i] != '\n'){
		
		if(row == 0){
			s->instructions[i] = line[i] == 'L' ? 0 : 1;
			s->num_instructions++;
		}
		
		if(isalpha(line[i])){
			substr[j] = line[i];
			j++;
		}
		else{
			if(j == 3){
				
				char str[LEN+1];
				memset(str, 0, LEN+1);
				strncpy(str, substr, LEN);
				switch(found){
					
					case 0:
						strcpy(s->lookup[s->num_nodes], str);
						break;
					
					case 1:
						strcpy(s->nodes[s->num_nodes].str_left, str);
						break;
					
					case 2:
						strcpy(s->nodes[s->num_nodes].str_right, str);
						s->num_nodes++;
						break;
					
					default:
						printf("Invalid state!\n");
						exit(-1);
				}
				found++;
				j = 0;
				memset(substr, 0, MAXINSTR);
			}
		}
		i++;
	}
}

int label_to_id(State s, char* label){
	for(int i = 0; i < s.num_nodes; i++){
		if(strcmp(s.lookup[i], label) == 0){
			return i;
		}
	}
}

void set_start(State s, int* start, int* nstart){
	
	for(int i = 0; i < s.num_nodes; i++){
		if (s.lookup[i][2] == 'A'){
			start[*nstart] = i; 
			(*nstart)++;
		}
	}
}

int goal_reached(State s, int* current, int n){
	for(int i = 0; i < n; i++){
		if (s.lookup[current[i]][2] != 'Z'){
			return 0;
		}
	}
	return 1;
}

int score1(State s){
	
	long long step = 0;
	char goal[] = "ZZZ", start[] = "AAA";
	int current = label_to_id(s, start);
	
	while(strcmp(s.lookup[current], goal) != 0){
		
		int instr = s.instructions[step%s.num_instructions];
		current = instr ? s.nodes[current].right : s.nodes[current].left;
		step++;
	}
	return step;
}

long long score2(State s){
	
	int start[MAXNODES];
	int nstart = 0;
	set_start(s, start, &nstart);
	int steps[nstart];
	for(int i = 0; i < nstart; i++){
		steps[i] = 0;
	}
	
	for(int i = 0; i < nstart; i++){
		int current = label_to_id(s, s.lookup[start[i]]);
		while(s.lookup[current][2] != 'Z'){
			
			int instr = s.instructions[steps[i]%s.num_instructions];
			current = instr ? s.nodes[current].right : s.nodes[current].left;
			steps[i]++;
		}
	}
	
	return LCMM(steps, nstart);
}

void set_neighbor_ids(State* s){
	
	for(int i = 0; i < s->num_nodes; i++){
		s->nodes[i].left = label_to_id(*s, s->nodes[i].str_left);
		s->nodes[i].right = label_to_id(*s, s->nodes[i].str_right);
	}
}

int main(int argc, char *argv[]){
  
  char* fname = "data/day8.txt";
  State s = {.num_nodes = 0};
	init_arr(s.instructions, MAXINSTR, 0);
  get_lines(fname, handle_line, &s);
	set_neighbor_ids(&s);
	printf("Score1: %d\n", score1(s));
	printf("Score2: %lld\n", score2(s));
  
  return 0;
}