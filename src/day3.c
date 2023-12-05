#include "common.h"
#include <ctype.h>

#define DIM 140
#define MSIZE DIM*DIM

typedef struct Symbol{
	int pos;
	char c;
}Symbol;

typedef struct Number{
	int pos;
	int length;
	int value;
}Number;

typedef struct State{
	Symbol* symbols;
	Number* numbers;
	int num_symbols;
	int num_numbers;
}State;

void set_number(State* s, char* str, int row, int current, int len){
	
	s->numbers[s->num_numbers].value = atoi(str);
	s->numbers[s->num_numbers].pos = row*DIM + current-len;
	s->numbers[s->num_numbers].length = len;
	s->num_numbers++;
	memset(str, 0, 10);
}

void set_symbol(State* s, char c, int row, int current){
	
	s->symbols[s->num_symbols].c = c;
	s->symbols[s->num_symbols].pos = row*DIM + current;
	s->num_symbols++;
}

int score1(State s){
	
	int sum = 0;
	for(int i = 0; i < s.num_numbers; i++){
		for(int j = 0; j < s.numbers[i].length; j++){
			int found = 0;
			for(int k = 0; k < s.num_symbols; k++){
				if (adjacent_asm(s.numbers[i].pos + j, s.symbols[k].pos, DIM)){
					sum += s.numbers[i].value;
					found++;
					break;
				}
			}
			if (found){
				break;
			}
		}
	}
	return sum;
}

int score2(State s){
	
	int sum = 0;
	for(int i = 0; i < s.num_symbols; i++){
		if (s.symbols[i].c == '*'){
			int neighbors = 0;
			int ratio = 1;
			for (int j = 0; j < s.num_numbers; j++){
				if (neighbors > 2){
					break;
				}
				for(int k = 0; k < s.numbers[j].length; k++){
					if (adjacent_asm(s.symbols[i].pos, s.numbers[j].pos + k, DIM)){
						ratio *= s.numbers[j].value;
						neighbors++;
						break;
					}
				}
			}
			if (neighbors == 2){
				sum += ratio;
			}
		}
	}
	return sum;
}

void handle_line(char* line, int row, void* params){
	
	State* s = (State*) params;
	int i = 0, j = 0;
	char substr[10];
	memset(substr, 0, 10);
	
	while(1){
		if (line[i] == '\n' || line[i] == 0){
			if (j > 0){
				set_number(s, substr, row, i, j);
				j = 0;
			}
			break;
		}
		else if (isdigit(line[i])){
			substr[j] = line[i];
			j++;
		}
		else if (line[i] == '.'){
			if (j > 0){
				set_number(s, substr, row, i, j);
				j = 0;
			}
		}
		else{
			set_symbol(s, line[i], row, i);
			if (j > 0){
				set_number(s, substr, row, i, j);
				j = 0;
			}
		}
		i++;
	}
}

void init_state(State* s){
	s->symbols = malloc(MSIZE * sizeof(Symbol));
	s->numbers = malloc(MSIZE * sizeof(Number));
	if (!s->symbols || !s->numbers){
		printf("OOM!\n");
		exit(-1);
	}
	s->num_symbols = 0;
	s->num_numbers = 0;
}

void free_state(State* s){
	free(s->symbols);
	free(s->numbers);
}

int main(int argc, char *argv[]){
  
  char* fname = "data/day3.txt";
	State s;
	init_state(&s);
	
  get_lines(fname, handle_line, &s);
	int part1 = score1(s), part2 = score2(s);
	printf("Part1: %d, part2: %d\n", part1, part2);
	free_state(&s);
  return 0;
}