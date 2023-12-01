#include "common.h"
#include <ctype.h>
#include <string.h>

void line_handler1(char* line, void* sum){
  
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

// Yikes
int find_string_number(char* string){
  
  if (strstr(string, "one") != NULL){
    return 1;
  }
  if (strstr(string, "two") != NULL){
    return 2;
  }
  if (strstr(string, "three") != NULL){
    return 3;
  }
  if (strstr(string, "four") != NULL){
    return 4;
  }
  if (strstr(string, "five") != NULL){
    return 5;
  }
  if (strstr(string, "six") != NULL){
    return 6;
  }
  if (strstr(string, "seven") != NULL){
    return 7;
  }
  if (strstr(string, "eight") != NULL){
    return 8;
  }
  if (strstr(string, "nine") != NULL){
    return 9;
  }
  return 0;
}

// Abomination
int find_string_number_rev(char* string){
  
  if (strstr(string, "eno") != NULL){
    return 1;
  }
  if (strstr(string, "owt") != NULL){
    return 2;
  }
  if (strstr(string, "eerht") != NULL){
    return 3;
  }
  if (strstr(string, "ruof") != NULL){
    return 4;
  }
  if (strstr(string, "evif") != NULL){
    return 5;
  }
  if (strstr(string, "xis") != NULL){
    return 6;
  }
  if (strstr(string, "neves") != NULL){
    return 7;
  }
  if (strstr(string, "thgie") != NULL){
    return 8;
  }
  if (strstr(string, "enin") != NULL){
    return 9;
  }
  return 0;
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
    
    int num = find_string_number(substr);
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
    int num = find_string_number_rev(substr);
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


void line_handler2(char* line, void* sum){
  
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
    get_lines(fname, line_handler1, &sum);
  }
  else if(strcmp(argv[1], "part2") == 0){
    get_lines(fname, line_handler2, &sum);
  }
  else{
    printf("Invalid args!\n");
    exit(-1);
  }
  
  printf("Sum: %d\n", sum);
  return 0;
}