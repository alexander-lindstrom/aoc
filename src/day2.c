#include "common.h"
#include <string.h>

typedef struct Cubes {
	int red;
	int green;
	int blue;
} Cubes;

typedef struct Game {
	int id;
	int subgames;
	Cubes* cubes;
} Game;

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
	
	if (strstr(str, "red") != NULL){
		g->cubes[cube_index].red = num;
	}
	else if (strstr(str, "green") != NULL){
		g->cubes[cube_index].green = num;
	}
	else if (strstr(str, "blue") != NULL){
		g->cubes[cube_index].blue = num;
	}
	else{
		printf("Invalid color!\n");
		exit(-1);
	}
}

void set_colors(Game* g, const char* str, int cube_index){
	
	char substr[BUFSIZE];
	memset(substr, 0, BUFSIZE);
	int i = 0;
	int j = 0;
	int num = -1;
	
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
	
	int rmax = 12;
	int gmax = 13;
	int bmax = 14;

	for(int i = 0; i < g.subgames; i++){
		if (rmax < g.cubes[i].red){
			return 0;
		}
		if (gmax < g.cubes[i].green){
			return 0;
		}
		if (bmax < g.cubes[i].blue){
			return 0;
		}
	}
	return g.id;
}

void handle_line(char* line, void* sum, int (*score)(Game)){
	
	//Init
	Game g; 
	g.subgames = count_chars(line, ';') + 1;
	g.id = get_id(line);
	g.cubes = (Cubes*)calloc(g.subgames, sizeof(Cubes));
	if(!g.cubes){
		printf("Out of mana!\n");
		exit(-1);
	}

	parse(line, &g);
	int* isum = (int*) sum;
	*isum += score(g);
	free(g.cubes);
}

int minimum_power(Game g){
	
	int rmax = 0;
	int gmax = 0;
	int bmax = 0;
	for(int i = 0; i < g.subgames; i++){
		if (g.cubes[i].red > rmax){
			rmax = g.cubes[i].red;
		}
		if (g.cubes[i].green > gmax){
			gmax = g.cubes[i].green;
		}
		if (g.cubes[i].blue > bmax){
			bmax = g.cubes[i].blue;
		}
	}
	return rmax*gmax*bmax;
}

void part1(char* line, void* sum){
	handle_line(line, sum, possible);
}

void part2(char* line, void* sum){
	handle_line(line, sum, minimum_power);
}

int main(int argc, char *argv[]){
  
  int sum = 0;
  char* fname = "data/day2.txt";
  
  if(strcmp(argv[1], "part1") == 0){
    get_lines(fname, part1, &sum);
  }
  else if(strcmp(argv[1], "part2") == 0){
    get_lines(fname, part2, &sum);
  }
  
  printf("Sum: %d\n", sum);
  return 0;
}