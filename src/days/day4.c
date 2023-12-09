#include "../common.h"

#define MAXDIGITS 10
#define NUMWINNING 10
#define NUMNUMBERS 25
#define NUMCARDS 193

typedef struct Card{
  int winning[NUMWINNING];
  int numbers[NUMNUMBERS];
}Card;

typedef struct State{
  Card cards[NUMCARDS];
}State;

void handle_line(char* line, int row, void* params){
	
  State* s = (State*) params;
  int flag = 0;
  char substr[MAXDIGITS];
  memset(substr, 0, MAXDIGITS);
  
  int i = 0, j = 0, k = 0;
  while(1){
		if (line[i] == '\n' || line[i] == 0){
      s->cards[row].numbers[k] = atoi(substr);
      k++;
      j = 0;
      memset(substr, 0, MAXDIGITS);
      break;
		}
    else if (line[i] == ' ' && j > 0){
      if (flag){
        s->cards[row].numbers[k] = atoi(substr);
      }
      else{
        s->cards[row].winning[k] = atoi(substr);
      }
      k++;
      j = 0;
      memset(substr, 0, MAXDIGITS);
		}
		else if (isdigit(line[i])){
			substr[j] = line[i];
			j++;
		}
		else if (line[i] == ':'){
      j = 0;
      memset(substr, 0, MAXDIGITS);
		}
		else if (line[i] == '|'){
      flag = 1;
      k = 0;
    }
		i++;
	}
}

int score1 (State s){
  
  int score = 0;
  for(int i = 0; i < NUMCARDS; i++){
    int matches = intersection_size(s.cards[i].winning, s.cards[i].numbers, NUMWINNING, NUMNUMBERS);
    score += matches ? (1 << (matches - 1)) : 0;
  }
  return score;
}

int score2 (State s){
  
  int copies[NUMCARDS];
  init_arr(copies, 1, NUMCARDS);
  for(int i = 0; i < NUMCARDS; i++){
    int matches = intersection_size(s.cards[i].winning, s.cards[i].numbers, NUMWINNING, NUMNUMBERS);
    for(int j = 0; j < copies[i]; j++){
      for(int k = 1; k <= matches; k++){
        copies[i + k]++;
      }
    }
  }
  return sum_arr(copies, NUMCARDS);
}

int main(int argc, char *argv[]){
  
  char* fname = "data/day4.txt";
  State s;
  get_lines(fname, handle_line, &s);
	printf("Part1: %d, part2: %d\n", score1(s), score2(s));
  return 0;
}