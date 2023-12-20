#include "../common.h"
#include <stdbool.h>
#include <assert.h>

#define BROADCASTER 0
#define FLIPFLOP 1
#define CONJUNCTION 2

#define LOW 0
#define HIGH 1

#define OFF 0
#define ON 1

#define MAXMODULES 100
#define MAXNB 10
#define MAXLABEL 16

#define KLSOURCES 4

typedef struct Module{
    int type;
    int state; //flip-flop
    int latestSent;
    int numDest;
    int numSrc;
    char label[MAXLABEL];
    char destLabels[MAXNB][MAXLABEL];
    void* destinations[MAXNB];
    void* sources[MAXNB];
}Module;

typedef struct Pulse{
    int type;
    Module* dest;
}Pulse;

typedef struct Node{
    Pulse p;
    struct Node* next;
}Node;

typedef struct CycleLengths{
    char labels[KLSOURCES][MAXLABEL];
    int64_t lengths[KLSOURCES];
}CycleLengths;

typedef struct State{
    int numModules;
    Module modules[MAXMODULES];
    CycleLengths cycles;
}State;

void enqueue(Node** head, Pulse p){
    Node* new_node = calloc(1, sizeof(Node));
    assert(new_node);

    new_node->p = p;
    new_node->next = *head;
    *head = new_node;
}

Pulse dequeue(Node** head){
    Node *current, *prev = NULL;
    assert(*head);
    current = *head;

    while(current->next != NULL){
        prev = current;
        current = current->next;
    }

    Pulse retval = current->p;
    free(current);
    
    if(prev){
        prev->next = NULL;
    }
    else{
        *head = NULL;
    }
    return retval;
}

bool isempty(Node** head){
    return *head == NULL;
}

void print_list(Node* head) {
    Node* current = head;
    while(current != NULL) {
        printf("Destination: %s, type: %d\n", current->p.dest->label, current->p.type);
        current = current->next;
    }
}

void handleLine(char* line, int row, void* params){
	
    State* s = (State*) params;
    char substr[MAXLABEL];
    memset(substr, 0, MAXLABEL);

    //Lazy: makes broadcaster -> roadcaster
    int i = 1, j = 0;
    if(line[0] == '%'){
        s->modules[s->numModules].type = FLIPFLOP;
    }
    else{
        s->modules[s->numModules].type = line[0] == '&' ? CONJUNCTION : BROADCASTER;
    }

    while(line[i] != 0 && line[i] != '\n'){
        switch (line[i]){
        case '-':
            substr[j-1] = 0;
            strcpy(s->modules[s->numModules].label, substr);
            i += 2;
            j = 0;
            memset(substr, 0, MAXLABEL);
            break;
        case ',':
            strcpy(s->modules[s->numModules].destLabels[s->modules[s->numModules].numDest],
                substr);
            i++;
            j = 0;
            memset(substr, 0, MAXLABEL);
            s->modules[s->numModules].numDest++;
            break;
        default:
            substr[j] = line[i];
            j++;
        }
        i++;
    }
    strcpy(s->modules[s->numModules].destLabels[s->modules[s->numModules].numDest],
        substr);
    s->modules[s->numModules].numDest++;
    s->numModules++;
}

void setup(State* s){

    for(int i = 0; i < s->numModules; i++){
        const char* iLabel = s->modules[i].label;
        for(int j = 0; j < s->numModules; j++){
            const char* jLabel = s->modules[j].label;
            if(i == j){
                continue;
            }
            for(int k = 0; k < s->modules[i].numDest; k++){
                if(strcmp(jLabel, s->modules[i].destLabels[k]) == 0){
                    s->modules[j].sources[s->modules[j].numSrc] = &s->modules[i];
                    s->modules[j].numSrc++;
                }
            }
            for(int k = 0; k < s->modules[j].numDest; k++){
                if(strcmp(iLabel, s->modules[j].destLabels[k]) == 0){
                    s->modules[j].destinations[k] = &s->modules[i];
                }
            }
        }
    }
}

Module* labelToModulePtr(State* s, char* label){
    for(int i = 0; i < s->numModules; i++){
        if(strcmp(s->modules[i].label, label) == 0){
            return &s->modules[i];
        }
    }
    assert(false);
}

void push(State* s, int* low, int* high, int iter, int part){

    Node* head = NULL;
    Pulse initial;
    initial.type = LOW, initial.dest = labelToModulePtr(s, "roadcaster");
    enqueue(&head, initial);

    while(!isempty(&head)){

        Pulse cur = dequeue(&head);
        Module* m = cur.dest;
        int nextType;
        if(m->type == FLIPFLOP){
            if(cur.type == LOW){
                m->state = m->state == ON ? OFF : ON;
                nextType = m->state == ON ? HIGH : LOW;
            }
            else{
                // flip-flop ignores high pulses
                continue;
            }
        }
        else{
            for(int i = 0; i < m->numSrc; i++){
                Module* src = (Module*) m->sources[i];
                if(src->latestSent != HIGH){
                    nextType = HIGH;
                    goto done;
                }
            }
            nextType = LOW;
        }
        done:
        
        //Send pulses
        m->latestSent = nextType;
        for(int i = 0; i < m->numDest; i++){
            Module* dest = (Module*) m->destinations[i];
            if(nextType == HIGH){
                (*high)++;
            }
            else{
                (*low)++;
            }
            if(!dest){
                continue;
            }
            Pulse next = {.type = nextType, .dest = dest};
            enqueue(&head, next);
            if(part == 2){
                for(int i = 0; i < KLSOURCES; i++){
                    if(strcmp(m->label, s->cycles.labels[i]) == 0 && next.type == HIGH && s->cycles.lengths[i] == 0){
                        s->cycles.lengths[i] = iter;
                    }
                }
            }
        }
    }
}

int score1(char* fname){
    
    State* s = calloc(1, sizeof(State));
    assert(s);
    get_lines(fname, handleLine, s);
    setup(s);
    
    int low = 0, high = 0;
    for(int i = 0; i < 1000; i++){
        low++; // Button push itself
        push(s, &low, &high, i, 1);
    }
    free(s);
    return low * high;
}

int64_t score2(char* fname){
    
    State* s = calloc(1, sizeof(State));
    assert(s);
    get_lines(fname, handleLine, s);
    setup(s);
    char klSources[KLSOURCES][MAXLABEL] = {"mk", "fp", "xt", "zc"};
    for(int i = 0; i < KLSOURCES; i++){
        strcpy(s->cycles.labels[i], klSources[i]);
    }

    int low = 0, high = 0;
    int i = 0;
    for(i = 0; i < 1e5; i++){
        low++; // Button push itself
        push(s, &low, &high, i+1, 2);
    }
    int64_t LCM = LCMM(s->cycles.lengths, KLSOURCES);
    free(s);
    return LCM;
}

int main(int argc, char *argv[]){
  
    char* fname = "data/day20.txt";
    printf("Score1: %d\n", score1(fname));
    printf("Score1: %lld\n", score2(fname));
    return 0;
}