#include "common.h"
#include <ctype.h>
#include <string.h>

char numbers[9][6] = {"one", "two", "three", "four", "five",
  "six", "seven", "eight", "nine"};
  
char reversed[9][6] = {"eno", "owt", "eerht", "ruof", "evif",
  "xis", "neves", "thgie", "enin"};
  
void handle_line1(char* line, int row, void* sum){
  
  int digits[BUFSIZE];
  int i = 0;
  int digit_index = 0;
  
  while(1){
    
    if (line[i] == '\n' || line[i] == 0){
      break;
    }
    
    if (isdigit(line[i])){
      digits[digit_index] = line[i];
      digit_index++;
    }
    i++;
  }
  
  char string[3] = {digits[0], digits[digit_index-1], '\0'};
  int* isum = (int*) sum;
  *isum += atoi(string);
}

int find_string_number(char* string, char lookup[9][6]){
  
  for (int i=0; i<9; i++){
    if (strstr(string, lookup[i]) != NULL){
      return i+1;
    }
  }
}

int get_first(char* line){
  
  int i = 0;
  char substr[BUFSIZE];
  memset(substr, 0, BUFSIZE);
  while(1){
    
    if (line[i] == '\n' || line[i] == 0){
      break;
    }
    substr[i] = line[i];
    
    int num = find_string_number(substr, numbers);
    if(num){
      return num;
    }
    
    if (isdigit(line[i])){
      return line[i] - '0';
    }
    
    i++;
  }
  return 0;
}

int get_last(char* line){
  
  int i = 0;
  int index = 0;
  char substr[BUFSIZE];
  memset(substr, 0, BUFSIZE);
  
  while(1){
    if (line[i] == '\n' || line[i] == 0){
      break;
    }
    i++;
  }
  
  for(int j = i; j >=0; j--){
    substr[index] = line[j];
    int num = find_string_number(substr, reversed);
    if(num){
      return num;
    }
    
    if (isdigit(line[j])){
      return line[j] - '0';
    }
    index++;
  }
  return 0;
}


void handle_line2(char* line, int row, void* sum){
  
  int first = get_first(line);
  int last = get_last(line);
  
  char string[3] = {first + '0', last + '0', '\0'};
  int* isum = (int*) sum;
  *isum += atoi(string);
}

int main(int argc, char *argv[]){
  
  if (argc != 2){
    printf("Usage: dayn partx");
    exit(-1);
  }
  
  int sum = 0;
  char* fname = "data/day1.txt";
  
  if(strcmp(argv[1], "part1") == 0){
    get_lines(fname, handle_line1, &sum);
  }
  else if(strcmp(argv[1], "part2") == 0){
    get_lines(fname, handle_line2, &sum);
  }
  else{
    printf("Invalid args!\n");
    exit(-1);
  }
  
  printf("Sum: %d\n", sum);
  return 0;
}