#include "../common.h"

#define MAXINSTR 1000
#define MAXHISTORY 1000
#define MAXHEX 16

typedef struct Point{
    long long row;
    long long col;
}Point;

typedef struct Instruction{
	char dir;
	int value;
    char hex[MAXHEX];

}Instruction;

typedef struct State{
    Instruction instructions[MAXINSTR];
	int numInstructions;
    int numHistory;
	Point history[MAXHISTORY];
}State;

char mapping[] = {'R', 'D', 'L', 'U'};

void handle_line(char* line, int row, void* params){
	
    State* s = (State*) params;
    s->instructions[s->numInstructions].dir = line[0]; 
    int i = 2, j = 0;
    char substr[MAXHEX];
    memset(substr, 0, MAXHEX);

	while(line[i] != ' '){
        substr[j] = line[i];
        j++;
	    i++;
	}
    i++;
    s->instructions[s->numInstructions].value = atoi(substr);
    memmove(s->instructions[s->numInstructions].hex, line + i + 2, 6);
    s->numInstructions++;
}

long long score(State* s, int part){
    Point cur = {0}, next;
    long long b = 0, A = 0;

    for(int i = 0; i < s->numInstructions; i++){

        Instruction I = s->instructions[i];
        int dir = I.dir;
        int value = I.value;
        if(part == 2){
            dir = mapping[I.hex[5] - '0'];
            I.hex[5] = 0;
            char* dummy;
            value = strtol(I.hex, &dummy, 16);
        }
        
        b += value;
        switch(dir){
            case 'R':
                next.row = 0, next.col = value;
                break;
            case 'D':
                next.row = value, next.col = 0;
                break;
            case 'L':
                next.row = 0, next.col = -value;
                break;
            case 'U':
                next.row = -value, next.col = 0;
                break;
        }
        A += cur.col * next.row - cur.row * next.col;
        cur.row += next.row, cur.col += next.col;
    }
    return  0.5 * b + 0.5 * A + 1;
}

int main(int argc, char *argv[]){
  
    char* fname = "data/day18.txt";
    State s = {0};
    get_lines(fname, handle_line, &s);
    printf("Score1: %lld\n", score(&s, 1));
    printf("Score2: %lld\n", score(&s, 2));
    return 0;
}