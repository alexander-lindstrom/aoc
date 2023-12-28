#include "../common.h"
#include <stdbool.h>
#include <assert.h>
#include <time.h>

#define MAXLABEL 8
#define MAXNUMNODES 2*1283
//#define MAXNUMNODES 2*13
#define MAXEDGES 10

typedef struct Node{
    char label[MAXLABEL];
    int numEdges;
    char edges[MAXEDGES][MAXLABEL];
}Node;

typedef struct State{
    Node nodes[MAXNUMNODES];
    int adj[MAXNUMNODES][MAXNUMNODES];
    int numNodes;
    int totalEdges;
}State;

bool nodeExists(State* s, const char* label){
    for(int i = 0; i < s->numNodes; i++){
        if(strncmp(label, s->nodes[i].label, 3) == 0){
            return true;
        }
    }
    return false;
}

void addNode(State* s, const char* label){
    if(!nodeExists(s, label)){
        strncpy(s->nodes[s->numNodes].label, label, 3);
        s->numNodes++;
    }
}

int labelToId(State* s, char* label){
    if(s->numNodes == 0){
        return 0;
    }
    for(int i = 0; i < s->numNodes; i++){
        if(strncmp(label, s->nodes[i].label, 3) == 0){
            return i;
        }
    }
    assert(false);
}

void handleLine(char* line, int row, void* params){
	
    State* s = (State*) params;
    char* token;
    token = strtok(line, " ");
    addNode(s, token);
    int currNode = labelToId(s, token);
    while(true){

        token = strtok(NULL, " ");
        if(token == NULL){
            break;
        }
        
        addNode(s, token);
        Node* n = &s->nodes[currNode];
        strncpy(n->edges[n->numEdges], token, 3);
        n->numEdges++;
    }
}

void setAdjMat(State* s){
    for(int i = 0; i < s->numNodes; i++){
        for(int j = 0; j < s->nodes[i].numEdges; j++){
            for(int k = 0; k < s->numNodes; k++){
                if(strcmp(s->nodes[i].edges[j], s->nodes[k].label) == 0){
                    s->adj[i][k] = 1;
                    s->adj[k][i] = 1; //Undirected
                    s->totalEdges++;
                }
            }
        }
    }
}

int totalEdges(State* s, int* unusable){
    int count = 0;
    for(int i = 0; i < s->numNodes; i++){
        for(int j = i; j < s->numNodes; j++){
            if(!unusable[i] && !unusable[j]){
                count += s->adj[i][j];
            }
        }
    }
    return count;
}

void contract(State* s, int* unusable, int* represent, int* remainingNodes){

    int r = rand() % (s->totalEdges - 0) + 0; // [0, totalEdges-1]
    int count = 0, i, j;
    for(i = 0; i < s->numNodes; i++){
        for(j = i; j < s->numNodes; j++){
            if(!unusable[i] && !unusable[j]){
                count += s->adj[i][j];
            }
            if(count >= r){
                goto done;
            }
        }
    }
    assert(false);
    done:;

    for(int k = 0; k < s->numNodes; k++){
        if(!unusable[k]){
            s->adj[j][k] += s->adj[i][k];
            s->adj[k][j] += s->adj[k][i];
        }
    }
    s->adj[j][j] = 0;
    unusable[i] = 1;
    represent[j] += represent[i];
    s->totalEdges = totalEdges(s, unusable);
    (*remainingNodes)--;
}

int groupProd(State* s, int* unusable, int* represent){
    int prod = 1;
    for(int i = 0; i < s->numNodes; i++){
        if(!unusable[i]){
            prod *= represent[i];
            printf("repesent: %d\n", represent[i]);
        }
    }
    return prod;
}

int iteration(State* s){

    int remainingNodes = s->numNodes;
    int unusable[MAXNUMNODES], represent[MAXNUMNODES];
    init_arr(unusable, 0, MAXNUMNODES);
    init_arr(represent, 1, MAXNUMNODES);

    while(remainingNodes > 2){
        contract(s, unusable, represent, &remainingNodes);
    }
    if(s->totalEdges == 3){
        return groupProd(s, unusable, represent);
    }
    printf("Cutsize: %d\n", s->totalEdges);
    return 0;
}

// Refer to: https://people.engr.tamu.edu/j-chen3/courses/658/2016/notes/s1.pdf
// Seems to run in a few minutes. Could be made much faster even without using the Karger-Stein improvement.
int score(State* s){

    int iter = 10*s->numNodes*s->numNodes;
    for(int i = 0; i < iter; i++){
        printf("i: %d\n",i);
        State* dummy = calloc(1, sizeof(State));
        assert(dummy);
        memcpy(dummy, s, sizeof(State));
        int res = iteration(dummy);
        free(dummy);
        if(res){
            return res;
        }
    }
    assert(false);
}

int main(int argc, char *argv[]){
  
    State *s = calloc(1, sizeof(State));
    srand(time(NULL));
    assert(s);
    char* fname = "data/day25.txt";
    get_lines(fname, handleLine, s);
    setAdjMat(s);
    printf("Score1: %d\n", score(s));
    free(s);
    return 0;
}