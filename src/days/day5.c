#include "../common.h"

#define MAX_SEEDS 50
#define MAX_MAPS 10
#define MAX_ROWS 50
#define ROW_LEN 3
typedef struct Row{
  long long items[ROW_LEN];
}Row;

typedef struct Map{
  int nrows;
  Row* rows;
}Map;

typedef struct State{
  int nseeds;
  long long* seeds;
  int nmaps;
  Map* maps;
}State;

void handle_line(char* line, int row, void* params){
	
  State* s = (State*) params;
  if (strstr(line, "seeds") != NULL){
    get_numbers_ll(line, s->seeds, &s->nseeds);
  }
  else if (strstr(line, "map:") != NULL){
    s->nmaps++;
  }
  else if (line[0] == '\n'){
    //skip empty line
  }
  else{
    int dummy = 0;
    get_numbers_ll(line, s->maps[s->nmaps-1].rows[s->maps[s->nmaps-1].nrows].items,
      &dummy);
    s->maps[s->nmaps-1].nrows++;
  }
}

long long get_src_mapping(long long n, Map* M){
  
  for (int i = 0; i < M->nrows; i++){
    long long a = M->rows[i].items[0], b = M->rows[i].items[1], c = M->rows[i].items[2];
    if(between(n, b, b + c)){
      return n + a - b;
    }
  }
  return n;
}

long long seed_location(State* s, long long seed){
  
  long long score = LLONG_MAX;
  long long value = seed;
  for(int j = 0; j < s->nmaps; j++){
    value = get_src_mapping(value, &s->maps[j]);
  }
  if (value < score){
    score = value;
  }
  return score;
}

long long score1(State s){
  
  long long score = LLONG_MAX;
  for(int i = 0; i < s.nseeds; i++){
    long long value = seed_location(&s, s.seeds[i]);
    if (value < score){
      score = value;
    }
  }
  return score;
}

//Very slow ~ 15 mins
long long score2(State s){
  
  long long score = LLONG_MAX;
  int best = -1;
  
  //Find the best pair
  for(int i = 0; i < s.nseeds; i += 2){
    long long s1 = s.seeds[i], s2 = s.seeds[i+1];
    for (long long k = s1; k < s1+s2; k += 1000){
      long long value = seed_location(&s, k);
      if(value < score){
        best = i;
        score = value;
      }
    }
  }
  score = LLONG_MAX;
  long long s1 = s.seeds[best], s2 = s.seeds[best+1];
  for (long long i = s1; i < s1+s2; i++){
    long long value = seed_location(&s, i);
    if(value < score){
      score = value;
    }
  }
  return score;
}

void init_state(State* s){
  
  s->maps = calloc(MAX_MAPS, sizeof(Map));
  s->seeds = calloc(MAX_SEEDS, sizeof(long long));
  if (!s->seeds || !s->maps){
    printf("OOM!\n");
    exit(-1);
  }
  for(int i = 0; i < MAX_MAPS; i++){
    s->maps[i].rows = calloc(MAX_ROWS, sizeof(Row));
    if (!s->maps[i].rows){
      printf("OOM!\n");
      exit(-1);
    }
  }
}

void free_state(State* s){
  for(int i = 0; i < MAX_MAPS; i++){
    free(s->maps[i].rows);
  }
  free(s->maps);
  free(s->seeds);
}

int main(int argc, char *argv[]){
  
  char* fname = "data/day5.txt";
  
  State s = {.nseeds = 0, .nmaps = 0};
  init_state(&s);
  get_lines(fname, handle_line, &s);
  printf("Part1: %lld\n", score1(s));
  printf("Part1: %lld\n", score2(s));
  free_state(&s);
  return 0;
}