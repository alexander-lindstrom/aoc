Advent of code 2023 in C/python

Compile examples:  
Day 2: ```gcc -g -std=c99 src/day2.c src/common.c src/common_c.c -o bin/day2```

Day 3 using assembly methods where applicable (x64 linux only):  
 ```nasm -felf64 src/common.asm -o bin/adj.o && gcc -g -std=c99 src/day3.c src/common.c bin/adj.o -o bin/day3```
