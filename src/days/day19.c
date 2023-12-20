#include "../common.h"
#include <stdbool.h>
#include <assert.h>

#define MAXRULES 10
#define MAXWORKFLOW 1000
#define MAXRATINGS 250
#define NUMTYPES 4
#define MAXLABEL 4
#define A -32
#define R -15

typedef struct Rule{
    char type;
    int sign;
    int value;
    char destLabel[MAXLABEL];
    int destId;
}Rule;

typedef struct Workflow{
    int numRules;
    Rule rules[MAXRULES];
    char defaultLabel[MAXLABEL];
    int defaultId;
    char label[MAXLABEL];
}Workflow;

typedef struct Rating{
    int values[NUMTYPES];
}Rating;

typedef struct cube{
    int min[NUMTYPES];
    int max[NUMTYPES];
}Cube;

typedef struct State{
    Workflow wf[MAXWORKFLOW];
    Rating ratings[MAXRATINGS];
    int flag;
    int numRatings;
    int numWorkflows;
    int numCubes;
    Cube cubes[MAXWORKFLOW];
}State;

char types[NUMTYPES] = {'x', 'm', 'a', 's'};

void handleLine(char* line, int row, void* params){
	
    State* s = (State*) params;
    if(line[0] == 0 || line[0] == '\n'){
        s->flag = 1;
        return;
    }
    
    if(!s->flag){
        int i = 0, j = 0, id = s->numWorkflows;
        char substr[16];
        memset(substr, 0, 16);
        while(line[i] != 0 && line[i] != '\n'){
            switch (line[i]){
            case '{':
                strncpy(s->wf[id].label, substr, MAXLABEL);
                j = 0;
                memset(substr, 0, 16);
                break;
            case '}':
                if(line[i-1] == 'A'){
                    s->wf[id].defaultId = A;
                }
                else if(line[i-1] == 'R'){
                    s->wf[id].defaultId = R;
                }
                strncpy(s->wf[id].defaultLabel, substr, MAXLABEL);
                j = 0;
                memset(substr, 0, 16);
                break;
            case ',':
                if(line[i-1] == 'A'){
                    s->wf[id].rules[s->wf[id].numRules].destId = A;
                }
                else if(line[i-1] == 'R'){
                    s->wf[id].rules[s->wf[id].numRules].destId = R;
                }
                strncpy(s->wf[id].rules[s->wf[id].numRules].destLabel, substr, MAXLABEL);
                s->wf[id].numRules++;
                j = 0;
                memset(substr, 0, 16);
                break;
            case ':':
                s->wf[id].rules[s->wf[id].numRules].value = atoi(substr);
                j = 0;
                memset(substr, 0, 16);
                break;
            case '<':
                s->wf[id].rules[s->wf[id].numRules].type = line[i-1];
                s->wf[id].rules[s->wf[id].numRules].sign = -1;
                j = 0;
                memset(substr, 0, 16);
                break;
            case '>':
                s->wf[id].rules[s->wf[id].numRules].type = line[i-1];
                s->wf[id].rules[s->wf[id].numRules].sign = 1;
                j = 0;
                memset(substr, 0, 16);
                break;  
            
            default:
                substr[j] = line[i];
                j++;
                break;
            }
            i++;
        }
        s->numWorkflows++;
    }
    else{
        int dummy = 0;
        get_numbers_int(line, s->ratings[s->numRatings].values, &dummy);
        s->numRatings++;
    }
}

void setWorkflowIds(State* s){

    for(int i = 0; i < s->numWorkflows; i++){
        for(int j = 0; j < s->numWorkflows; j++){
            if(strcmp(s->wf[i].label, s->wf[j].defaultLabel) == 0){
                s->wf[j].defaultId = i;
            }
            for(int k = 0; k < s->wf[j].numRules; k++){
                if(strcmp(s->wf[i].label, s->wf[j].rules[k].destLabel) == 0){
                 s->wf[j].rules[k].destId = i;
                }
            }
        }
    }
}

int workflowLabelToId(State* s, char* label){
    for(int i = 0; i < s->numWorkflows; i++){
        if(strcmp(s->wf[i].label, label) == 0){
            return i;
        }
    }
    assert(false);
}

int next(Workflow wf, Rating r){
    int dest;
    for(int i = 0; i < wf.numRules; i++){
        Rule rule = wf.rules[i];
        for(int j = 0; j < NUMTYPES; j++){
            if(types[j] == rule.type && sign(r.values[j] - rule.value) == rule.sign
                && r.values[j] != rule.value){
                dest = rule.destId;
                return dest;
            }
        }
    }
    return wf.defaultId;
}

int score1(State* s){
    int sum = 0;
    for(int i = 0; i < s->numRatings; i++){

        Rating r = s->ratings[i];
        int dest = WorkflowLabelToId(s, "in");
        while(dest != R && dest != A){
            dest = next(s->wf[dest], r);
        }
        if(dest == A){
            sum += sum_arr(r.values, NUMTYPES);
        }
    }
    return sum;
}

long long cubeVolume(Cube c1){
    long long volume = 1;
    for(int i = 0; i < NUMTYPES; i++){
        volume *= (long long)(c1.max[i] - c1.min[i]+1);
    }
    return volume;
}

void addHyperCube(State* s, int xmin, int xmax, int mmin, int mmax, int amin, int amax, int smin, int smax){
    s->cubes[s->numCubes].min[0] = xmin;
    s->cubes[s->numCubes].max[0] = xmax;
    s->cubes[s->numCubes].min[1] = mmin;
    s->cubes[s->numCubes].max[1] = mmax;
    s->cubes[s->numCubes].min[2] = amin;
    s->cubes[s->numCubes].max[2] = amax;
    s->cubes[s->numCubes].min[3] = smin;
    s->cubes[s->numCubes].max[3] = smax;
    s->numCubes++;
}

void traverse(State* s, int cur, int xmin, int xmax, int mmin, int mmax, 
    int amin, int amax, int smin, int smax){

    if(cur == A){
        addHyperCube(s, xmin, xmax, mmin, mmax, amin, amax, smin, smax);
        return;
    }
    else if(cur == R){
        return;
    }

    for(int i = 0; i < s->wf[cur].numRules; i++){
        if(s->wf[cur].rules[i].type == types[0]){
            if(s->wf[cur].rules[i].sign > 0){
                int nxmin = max(xmin, s->wf[cur].rules[i].value + 1);
                traverse(s, s->wf[cur].rules[i].destId, nxmin, xmax, mmin, mmax, amin, amax, smin, smax);
                xmax = min(xmax, s->wf[cur].rules[i].value);
            }
            else{
                int nxmax = xmax < s->wf[cur].rules[i].value ? xmax : s->wf[cur].rules[i].value - 1;
                traverse(s, s->wf[cur].rules[i].destId, xmin, nxmax, mmin, mmax, amin, amax, smin, smax);
                xmin = xmin >= s->wf[cur].rules[i].value ? xmin : s->wf[cur].rules[i].value;
            }
        }
        else if(s->wf[cur].rules[i].type == types[1]){
            if(s->wf[cur].rules[i].sign > 0){
                int nmmin = max(mmin, s->wf[cur].rules[i].value + 1);
                traverse(s, s->wf[cur].rules[i].destId, xmin, xmax, nmmin, mmax, amin, amax, smin, smax);
                mmax = mmax <= s->wf[cur].rules[i].value ? mmax : s->wf[cur].rules[i].value;
            }
            else{
                int nmmax = mmax < s->wf[cur].rules[i].value ? mmax : s->wf[cur].rules[i].value - 1;
                traverse(s, s->wf[cur].rules[i].destId, xmin, xmax, mmin, nmmax, amin, amax, smin, smax);
                mmin = mmin >= s->wf[cur].rules[i].value ? mmin : s->wf[cur].rules[i].value;
            }
        }
        else if(s->wf[cur].rules[i].type == types[2]){
            if(s->wf[cur].rules[i].sign > 0){
                int namin = amin > s->wf[cur].rules[i].value ? amin : s->wf[cur].rules[i].value + 1;
                traverse(s, s->wf[cur].rules[i].destId, xmin, xmax, mmin, mmax, namin, amax, smin, smax);
                amax = amax <= s->wf[cur].rules[i].value ? amax : s->wf[cur].rules[i].value;
            }
            else{
                int namax = amax < s->wf[cur].rules[i].value ? amax : s->wf[cur].rules[i].value - 1;
                traverse(s, s->wf[cur].rules[i].destId, xmin, xmax, mmin, mmax, amin, namax, smin, smax);
                amin = amin >= s->wf[cur].rules[i].value ? amin : s->wf[cur].rules[i].value;
            }
        }
        else if(s->wf[cur].rules[i].type == types[3]){
            if(s->wf[cur].rules[i].sign > 0){
                int nsmin = smin > s->wf[cur].rules[i].value ? smin : s->wf[cur].rules[i].value + 1;
                traverse(s, s->wf[cur].rules[i].destId, xmin, xmax, mmin, mmax, amin, amax, nsmin, smax);
                smax = smax <= s->wf[cur].rules[i].value ? smax : s->wf[cur].rules[i].value;
            }
            else{
                int nsmax = smax < s->wf[cur].rules[i].value ? smax : s->wf[cur].rules[i].value - 1;
                traverse(s, s->wf[cur].rules[i].destId, xmin, xmax, mmin, mmax, amin, amax, smin, nsmax);
                smin = smin >= s->wf[cur].rules[i].value ? smin : s->wf[cur].rules[i].value;
            }
        }
    }
    traverse(s, s->wf[cur].defaultId, xmin, xmax, mmin, mmax, amin, amax, smin, smax);
}

long long score2(State* s){

    int start = WorkflowLabelToId(s, "in");
    traverse(s, start, 1, 4000, 1, 4000, 1, 4000, 1, 4000);
    long long totalVolume = 0;
    for(int i = 0; i < s->numCubes; i++){
        totalVolume += cubeVolume(s->cubes[i]);
    }
    return totalVolume;
}

int main(int argc, char *argv[]){
  
    char* fname = "data/day19.txt";
    State* s = calloc(1, sizeof(State));
    assert(s);
    get_lines(fname, handleLine, s);
    setWorkflowIds(s);
    printf("Score1: %d\n", score1(s));
    printf("Score2: %lld\n", score2(s));
    free(s);
    return 0;
}