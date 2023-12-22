#include "../common.h"
#include <stdbool.h>
#include <assert.h>

#define DIM 131
#define ROCK -2
#define GRASS -1

#define DIRS 4
#define NORTH -DIM
#define SOUTH DIM
#define WEST -1
#define EAST 1

#define START 0
#define END DIM*DIM-1

int directions[DIRS] = {NORTH, EAST, SOUTH, WEST};

typedef struct Data{
    int pos;
    int dist;
}Data;

typedef struct State{
    int grid[DIM*DIM];
    int start;
}State;

typedef struct Node{
    Data data;
    struct Node* next;
}Node;

void enqueue(Node** head, Data data){
    Node* new_node = calloc(1, sizeof(Node));
    assert(new_node);

    new_node->data = data;
    new_node->next = *head;
    *head = new_node;
}

Data dequeue(Node** head){
    Node *current, *prev = NULL;
    assert(*head);
    current = *head;

    while(current->next != NULL){
        prev = current;
        current = current->next;
    }

    Data d = current->data;
    free(current);
    
    if(prev){
        prev->next = NULL;
    }
    else{
        *head = NULL;
    }
    return d;
}

bool isempty(Node** head){
    return *head == NULL;
}

void handleLine(char* line, int row, void* params){
	
    State* s = (State*) params;
    int i = 0;
    while(line[i] != 0 && line[i] != '\n'){
        switch (line[i]){
        case '.':
            s->grid[row*DIM + i] = GRASS;
            break;
        case '#':
            s->grid[row*DIM + i] = ROCK;
            break;
        case 'S':
            s->grid[row*DIM + i] = GRASS;
            s->start = row*DIM + i;
            break;
        }
        i++;
    }
}

void fill(int* grid, int start){

    Node* head = NULL;
    Data initial = {.pos = start, .dist = 0};
    grid[initial.pos] = initial.dist;
    enqueue(&head, initial);

    while(!isempty(&head)){

        Data cur = dequeue(&head);
        for(int i = 0; i < DIRS; i++){
            int nextPos = cur.pos + directions[i];
            //Avoid wrap
            if(directions[i] == EAST || directions[i] == WEST){
                if(nextPos/DIM != cur.pos/DIM){
                    continue;
                }
            }
            if(between(nextPos, START, END) && grid[nextPos] == GRASS){
                Data next = {.pos = nextPos, .dist = cur.dist + 1};
                grid[next.pos] = next.dist;
                enqueue(&head, next);
            }
        }
    }
}

int score1(State* s){

    int reachable = 0;
    for(int i = 0; i < DIM; i++){
        for(int j = 0; j < DIM; j++){
            int dist = s->grid[i*DIM + j];
            if(between(dist, 0, 64) && dist%2  == 0){
                reachable++;
            }
        }
    }
    return reachable;
}

long long score2(State* s){

    //Some kind of geomeric solution
    //Note that the number of steps is a multiple of the grid size
    long long steps = 26501365;
    long long numGrids = ((steps - (DIM / 2)) / DIM);
    //The "corners" outside the rhombus:
    long long evenOutside = 0, oddOutside = 0;
    //The full grid:
    long long allEven = 0, allOdd = 0;
    for(int i = 0; i < DIM; i++){
        for(int j = 0; j < DIM; j++){
            int value = s->grid[i*DIM + j];
            if(value < 0){
                continue;
            }
            if(value % 2 == 0){
                allEven++;
                if(value > 65){
                    evenOutside++;
                }
            }
            else{
                allOdd++;
                if(value > 65){
                    oddOutside++;
                }
            }
        }
    }
    return (numGrids+1)*(numGrids+1)*allOdd + numGrids*numGrids*allEven - 
        (numGrids+1)*oddOutside + numGrids*evenOutside;
}

int main(int argc, char *argv[]){
  
    State s = {0};
    char* fname = "data/day21.txt";
    get_lines(fname, handleLine, &s);
    fill(s.grid, s.start);
    printf("Score1: %d\n", score1(&s));
    printf("Score2: %lld\n", score2(&s));
    return 0;
}