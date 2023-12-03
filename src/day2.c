#include "common.h"

#define COLORS 3
char colors[][6] = {"red", "green", "blue"};

typedef struct Cubes{
	int quantity[COLORS]; //red, green, blue
} Cubes;

typedef struct Game{
	int id;
	int subgames;
	Cubes* cubes;
} Game;

typedef struct Params{
  int* s1;
  int* s2;
}Params;

int get_id(const char* line){

	char dummy[BUFSIZE];
	strcpy(dummy, line);
	
	const char delim[2] = ":";
	char* token;
	
	token = strtok(dummy, delim);
	
	//discard "Game: "
	return atoi(&token[5]);
}

void assign(char* str, Game* g, int num, int cube_index){
	
	for(int i = 0; i < COLORS; i++){
		if (strstr(str, colors[i]) != NULL){
			g->cubes[cube_index].quantity[i] = num;
			return;
		}
	}
	printf("Invalid color!\n");
	exit(-1);
}

void set_colors(Game* g, const char* str, int cube_index){
	
	char substr[BUFSIZE];
	memset(substr, 0, BUFSIZE);
	int i = 0, j = 0, num = 0;
	
	while(1){
		
		if(str[i] == ' '){
			num = atoi(substr);
			j = 0;
			memset(substr, 0, BUFSIZE);
		}
		else if (str[i] == ','){
			assign(substr, g, num, cube_index);
			j = 0;
			memset(substr, 0, BUFSIZE);
		}
		else if (str[i] == '\n' || str[i] == 0){
			assign(substr, g, num, cube_index);
			break;
		}
		substr[j] = str[i];
		i++;
		j++;
	}
}

void parse(const char* line, Game* g){

	char dummy[BUFSIZE];
	//discard the first part of the string
	char* second_part =  strchr(line, ':');
	//also the ':'
	strcpy(dummy, second_part+1);
	const char delim[2] = ";";
	char* token;
	
	token = strtok(dummy, delim);
	int i = 0;
	while(token != NULL){
		
		set_colors(g, token, i);
		token = strtok(NULL, delim);
		i++;
	}
}

int possible(Game g){
	
	int max[COLORS] = {12, 13, 14};
	for(int i = 0; i < g.subgames; i++){
		for(int j = 0; j < COLORS; j++){
			if (max[j] < g.cubes[i].quantity[j]){
				return 0;
			}
		}	
	}
	return g.id;
}

int minimum_power(Game g){
	
	int max[COLORS] = {0, 0, 0};
	for(int i = 0; i < g.subgames; i++){
		for(int j = 0; j < COLORS; j++){
			if (g.cubes[i].quantity[j] > max[j]){
				max[j] = g.cubes[i].quantity[j];
			}
		}
	}
	return max[0]*max[1]*max[2];
}

void handle_line(char* line, int row, void* params){
	
	Params* p = (Params*) params;
  int* score1 = p->s1;
  int* score2 = p->s2;
	Game g; 
	g.subgames = count_chars(line, ';') + 1;
	g.id = get_id(line);
	g.cubes = (Cubes*)calloc(g.subgames, sizeof(Cubes));
	if(!g.cubes){
		printf("Out of mana!\n");
		exit(-1);
	}

	parse(line, &g);
	*score1 += possible(g);
	*score2 += minimum_power(g);
	free(g.cubes);
}

int main(int argc, char *argv[]){
  
  int score1 = 0, score2 = 0;
  Params p;
  p.s1 = &score1, p.s2 = &score2;
  char* fname = "data/day2.txt";
  get_lines(fname, handle_line, &p);
  printf("Part1: %d, Part2: %d\n", score1, score2);
  return 0;
}