CC = gcc
CFLAGS = -g -std=c99

default:
	$(CC) $(CFLAGS) src/day$(day).c src/common.c src/common_c.c -o bin/day$(day)

asm:
	nasm -felf64 src/common.asm -o bin/common.o
	$(CC) $(CFLAGS) src/day$(day).c src/common.c bin/common.o -o bin/day$(day)
	
