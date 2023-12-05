Advent of code 2023 in C/python

Compile examples:  
Day n: ```gcc -g -std=c99 src/dayn.c src/common.c src/common_c.c -o bin/dayn```

Day 3 using assembly method(s):  
 ```nasm -felf64 src/common.asm -o bin/adj.o ```  
 ```gcc -g -std=c99 src/day3.c src/common.c src/common_asm.c bin/adj.o -o bin/day3```
