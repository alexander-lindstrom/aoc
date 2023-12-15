#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define BUFSIZE 1024*50
#define BOXES 256
#define LENSES_MAX 16
#define LABEL_MAX 16

typedef struct Pair{
	unsigned int value;
	char label[LABEL_MAX];
}Pair;

typedef struct Box{
	size_t size;
	Pair lenses[LENSES_MAX];
}Box;

typedef struct Hashmap{
	Box buckets[BOXES];
}Hashmap;

typedef struct State{
	Hashmap H;
	unsigned int sum;
}State;

unsigned int hash(char* label){
	
	int cur = 0, i = 0;
	while(label[i] != 0){
		cur = (17 * (cur + (unsigned int)label[i])) % BOXES;
		i++;
	}
	return cur;
}

void add_lens(Hashmap* H, char* label, unsigned int value){
	
	unsigned int bucket_index = hash(label);
	Box* box = &H->buckets[bucket_index];
	
	int i = 0;
	for(i; i < box->size; i++){
		
		//New value if label exists
		if(strcmp(box->lenses[i].label, label) == 0){
			box->lenses[i].value = value;
			return;
		}
	}
	
	//Add at end
	strcpy(box->lenses[i].label, label);
	box->lenses[i].value = value;
	box->size++;
}

void remove_lens(Hashmap* H, char* label){
	
	unsigned int bucket_index = hash(label);
	Box* box = &H->buckets[bucket_index];
	
	for(int i = 0; i < box->size; i++){
		
		//Remove and shuffle if label exists
		if(strcmp(box->lenses[i].label, label) == 0){
			for(int j = i; j < box->size-1; j++){
				memset(box->lenses[j].label, 0, LABEL_MAX);
				strcpy(box->lenses[j].label, box->lenses[j+1].label);
				box->lenses[j].value = box->lenses[j+1].value;
			}
			memset(box->lenses[box->size].label, 0, LABEL_MAX);
			box->lenses[box->size].value = 0;
			box->size--;
			return;
		}
	}
}

void get_line(char* fname, void (*line_handler)(char*, int, void*), 
	void* params){

	char buffer[BUFSIZE]; 
	FILE* fp = fopen(fname, "r");

	if(!fp){
		printf("Couldn't open file: %s\n", fname);
		exit(-1);
	}
	int i = 0;
	while(fgets(buffer, BUFSIZE, fp)){
		line_handler(buffer, i, params);
		i++;
	}
}

void handle_instruction(Hashmap* H, char* instruction){
	
	unsigned int i = 0;
	char substr[LABEL_MAX];
	memset(substr, 0, LABEL_MAX);
	while(i < LABEL_MAX && instruction[i] != 0){
		if(instruction[i] == '-'){
			remove_lens(H, substr);
		}
		else if(instruction[i] == '='){
			add_lens(H, substr, instruction[i+1] - '0');
		}
		else{
			substr[i] = instruction[i];
		}
		i++;
	}
}

void handle_line(char* line, int row, void* params){
	
	State* s = (State*) params;
	unsigned int i = 0, partial = 0, j = 0;
	char substr[LABEL_MAX];
	memset(substr, 0, LABEL_MAX);
	while(line[i] != 0 || line[i] == EOF){

		if(line[i] == '\n'){
    }
		else if(line[i]==','){
			s->sum += partial;
			partial = 0;
			handle_instruction(&s->H, substr);
			memset(substr, 0, LABEL_MAX);
			j = 0;
		}
		else{
			partial = (17 * (partial + (unsigned int) line[i])) % BOXES;
			substr[j] = line[i];
			j++;
		}
		i++;
	}
	handle_instruction(&s->H, substr);
  s->sum += partial;
}

unsigned int power(Hashmap H){
	
	unsigned int sum = 0;
	for(int i = 0; i < BOXES; i++){
		Box box = H.buckets[i];
		for(int j = 0; j < box.size; j++){
			sum += (i+1) * (j+1) * box.lenses[j].value;
		}
	}
	return sum;
}

int main(int argc, char *argv[]){
  
  char* fname = "data/day15.txt";
  State s = {0};
  get_line(fname, handle_line, &s);
  printf("Part1: %u\n", s.sum);
	printf("Part2: %u\n", power(s.H));
  return 0;
}