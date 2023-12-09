CC = gcc
CFLAGS = -g -std=c99

default:
	$(CC) $(CFLAGS) src/days/day$(day).c src/common.c src/ported.c -o bin/day$(day)

asm:
	nasm -felf64 src/ported.asm -o bin/ported.o
	$(CC) $(CFLAGS) src/days/day$(day).c src/common.c bin/ported.o -o bin/day$(day)
	
