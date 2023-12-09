#include "../common.h"
#define MAXITEMS 5

typedef struct State{
  int nt;
  int nd;
  int* time;
  int* distance;
}State;

void handle_line(char* line, int row, void* params){
	
  State* s = (State*) params;
  if (strstr(line, "Time:") != NULL){
    get_numbers_int(line, s->time, &s->nt);
  }
  else if (strstr(line, "Distance:") != NULL){
    get_numbers_int(line, s->distance, &s->nd);
  }
  else{
    printf("Invalid data!\n");
    exit(-1);
  }
}

long long travel_distance(long long time, long long windup){
  return (time-windup)*windup;
}

long long ways_to_win(long long time, long long record){
  
  int count = 0;
  for(int i = 0; i < time; i++){
    if (travel_distance(time, i) > record){
      count++;
    }
  }
  return count;
}

int score1(State s){
  int sum = 1;
  for(int i = 0; i < s.nt; i++){
    sum *= ways_to_win(s.time[i], s.distance[i]);
  }
  return sum;
}

int score2(State s){
  long long total_time = s.time[0];
  long long total_distance = s.distance[0];
  for(int i = 0; i < s.nt-1; i++){
    total_time = concat(total_time, s.time[i+1]);
    total_distance = concat(total_distance, s.distance[i+1]);
  }
  return ways_to_win(total_time, total_distance);
}

void init_state(State* s){
  s->time = calloc(MAXITEMS, sizeof(int));
  s->distance = calloc(MAXITEMS, sizeof(int));
  if(!s->time || !s->distance){
    printf("Alloc fail\n");
    exit(-1);
  }
}

void free_state(State* s){
  free(s->time);
  free(s->distance);
}

int main(int argc, char *argv[]){
  
  char* fname = "data/day6.txt";
  State s = {.nt = 0, .nd = 0};
  init_state(&s);
  get_lines(fname, handle_line, &s);
  printf("Score1: %d\n", score1(s));
  printf("Score2: %d\n", score2(s));
  free_state(&s);
  
  return 0;
}