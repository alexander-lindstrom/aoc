#include "../common.h"
#include <stdbool.h>
#include <assert.h>

#define XMAX 10
#define YMAX 10
#define ZMAX 400
#define DIMS 3
//#define NUMBLOCKS 1280
#define NUMBLOCKS 7
#define EMPTY 0
#define MAXBLOCKLEN 20

typedef struct Pos{
    int x;
    int y;
    int z;
}Pos;

typedef struct Block{
    Pos cubes[MAXBLOCKLEN];
    bool stopped;
    int id;
    int blockLen;
}Block;

typedef struct State{
    int grid[XMAX][YMAX][ZMAX];
    Block blocks[NUMBLOCKS];
}State;

bool onGrid(int x, int y, int z){
    return between(x, 0, XMAX-1) && between(y, 0, YMAX-1) && between(z, 0, ZMAX-1);
}

bool canMove(const int grid[XMAX][YMAX][ZMAX], const Block* b, int zDiff){
    for(int i = 0; i < b->blockLen; i++){
        int x = b->cubes[i].x, y = b->cubes[i].y, z = b->cubes[i].z;
        if(!onGrid(x, y, z+zDiff) || grid[x][y][z+ zDiff] != EMPTY){
            return false;
        }
    }
    return true;
}

//Overwrites without checking if allowed
void moveBlock(int grid[XMAX][YMAX][ZMAX], Block* b, int zDiff){
    for(int i = 0; i < b->blockLen; i++){
        grid[b->cubes[i].x][b->cubes[i].y][b->cubes[i].z] = EMPTY;
    }
    for(int i = 0; i < b->blockLen; i++){
        grid[b->cubes[i].x][b->cubes[i].y][b->cubes[i].z + zDiff] = b->id;
    }
}

void handleLine(char* line, int row, void* params){
	
    State* s = (State*) params;
    int dummy = 0;
    int arr[6];
    s->blocks[row].id = row+1; //Important: ID:s are 1-indexed
    get_numbers_int(line, arr, &dummy);

    if(arr[0] != arr[DIMS]){
        for(int i = arr[0]; i < arr[DIMS]; i++){
            s->blocks[row].cubes[s->blocks[row].blockLen].x = i;
            s->blocks[row].cubes[s->blocks[row].blockLen].y = arr[1];
            s->blocks[row].cubes[s->blocks[row].blockLen].z = arr[2];
            s->blocks[row].blockLen++;
        }
    }
    else if(arr[1] != arr[DIMS+1]){
        for(int i = arr[1]; i < arr[DIMS+1]; i++){
            s->blocks[row].cubes[s->blocks[row].blockLen].x = arr[0];
            s->blocks[row].cubes[s->blocks[row].blockLen].y = i;
            s->blocks[row].cubes[s->blocks[row].blockLen].z = arr[2];
            s->blocks[row].blockLen++;
        }
    }
    else if(arr[2] != arr[DIMS+2]){
        for(int i = arr[2]; i < arr[DIMS+2]; i++){
            s->blocks[row].cubes[s->blocks[row].blockLen].x = arr[0];
            s->blocks[row].cubes[s->blocks[row].blockLen].y = arr[1];
            s->blocks[row].cubes[s->blocks[row].blockLen].z = i;
            s->blocks[row].blockLen++;
        }
    }
    else{
        s->blocks[row].cubes[s->blocks[row].blockLen].x = arr[0];
        s->blocks[row].cubes[s->blocks[row].blockLen].y = arr[1];
        s->blocks[row].cubes[s->blocks[row].blockLen].z = arr[2];
    }
    moveBlock(s->grid, &s->blocks[row], 0);
}

int score1(State* s){

    int flag = 1;
    int iter = 0;
    while(flag){
        flag = 0;
        for(int i = 0; i < NUMBLOCKS; i++){
            printf("Iter: %d\n", iter);
            if(canMove(s->grid, &s->blocks[i], -1)){
                moveBlock(s->grid, &s->blocks[i], -1);
                flag = 1;
            }
        }
        iter++;
    }
    return 0;
}

int main(int argc, char *argv[]){
  
    State s = {0};
    char* fname = "data/day22_test.txt";
    get_lines(fname, handleLine, &s);
    printf("Score1: %d\n", score1(&s));
    //printf("Score2: %lld\n", score2(&s));
    return 0;
}