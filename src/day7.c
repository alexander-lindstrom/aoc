#include "common.h"
#define NUMCARDS 5
#define MAXROWS 1024
#define KINDS 13

char* order;
int part;

typedef struct Row{
	char* cards;
	int bid;
}Row;


typedef struct State{
	int numRows;
  Row* rows;
}State;

void handle_line(char* line, int row, void* params){
	
  State* s = (State*) params;
	
  const char delim[2] = " ";
	char* token;
	
	token = strtok(line, delim);
	strcpy(s->rows[row].cards, token);
	
	token = strtok(NULL, delim);
	s->rows[row].bid = atoi(token);
	s->numRows++;
}

int type_score1(char* cards){

	int max = max_occ_nc(cards, NUMCARDS, '1');
	switch(max){
		
		case 5:
			return 6;
		
		case 4:
			return 5;
			
		//full house, 3oc
		case 3:
			return num_unique(cards, NUMCARDS) == 2 ? 4 : 3;
		
		//2 pair, pair
		case 2:
			return num_unique(cards, NUMCARDS) == 3 ? 2 : 1;
		
		case 1:
			return 0;
	}
}

int type_score2(char* cards){

	int max_nj = max_occ_nc(cards, NUMCARDS, 'J');
	int c_nj = most_occ_nc(cards, NUMCARDS, 'J');
	
	// Only jokers
	if (max_nj == 0){
		return 6;
	}
	
	char dummy[NUMCARDS];
	strcpy(dummy, cards);
	int njoker = 0;
	for(int i = 0; i < NUMCARDS; i++){
		if (cards[i] == 'J'){
			dummy[i] = c_nj;
			njoker++;
		}
		else{
			dummy[i] = cards[i];
		}
	}
	
	int max = max_occ_nc(dummy, NUMCARDS, '1');
	switch(max){
		
		case 5:
			return 6;
		
		case 4:
			return 5;
			
		//full house, 3oc
		case 3:
			return num_unique(dummy, NUMCARDS) == 2 ? 4 : 3;
		
		//2 pair, pair
		case 2:
			return num_unique(dummy, NUMCARDS) == 3 ? 2 : 1;
		
		case 1:
			return 0;
	}
}

int larger(char c1, char c2){
	
	for(int i = 0; i < KINDS; i++){
		if (c1 == order[i] && c2 != order[i]){
			return 1;
		}
		if (c2 == order[i] && c1 != order[i]){
			return -1;
		}
	}
	return 0;
}

int cmp(const void* row1, const void* row2){
	
	Row r1 = *(Row*) row1;
	Row r2 = *(Row*) row2;
	char* cards1 = r1.cards;
	char* cards2 = r2.cards;
	
	int diff;
	if(part == 1){
		diff = type_score1(cards1) - type_score1(cards2);
	}
	else{
		diff = type_score2(cards1) - type_score2(cards2);
	}
	
	if (diff == 0){
		for(int i = 0; i < NUMCARDS; i++){
			if (cards1[i] != cards2[i]){
				return larger(cards1[i], cards2[i]);
			}
		}
	}
	return diff;
}

long long score(State s){
	
	qsort(s.rows, s.numRows, sizeof(Row), cmp);
	long long sum = 0, rank = 1;
	for(int i = 0; i < s.numRows; i++){
		sum += rank * s.rows[i].bid;
		rank++;
	}
	return sum;
}

void init_state(State* s){
  s->rows = calloc(MAXROWS, sizeof(Row));
  if(!s->rows){
    printf("Alloc fail\n");
    exit(-1);
  }
	for(int i = 0; i < MAXROWS; i++){
		s->rows[i].cards = calloc(NUMCARDS, sizeof(char));
		if(!s->rows[i].cards){
			printf("Alloc fail\n");
			exit(-1);
		}
	}
}

void free_state(State* s){
	for(int i = 0; i < MAXROWS; i++){
		free(s->rows[i].cards);
	}
  free(s->rows);
}

int main(int argc, char *argv[]){
  
	if(argc != 2){
		printf("Usage: day8 1/2\n");
		exit(-1);
	}
	char* fname = "data/day7.txt";
	State s = {.numRows = 0};
  init_state(&s);
  get_lines(fname, handle_line, &s);
	
	part = atoi(argv[1]);
	if(part == 1){
		char dummy[KINDS] = {'A', 'K', 'Q', 'J', 'T', '9', '8', '7', '6', '5', '4', '3', '2'};
		order = dummy;
		printf("Score1: %d\n", score(s));
	}
	else{
		char dummy[KINDS] = {'A', 'K', 'Q', 'T', '9', '8', '7', '6', '5', '4', '3', '2', 'J'};
		order = dummy;
		printf("Score2: %lld\n", score(s));
	}
  free_state(&s);
  return 0;
}