CC = gcc
CFLAGS = -g -std=c99

default:
	$(CC) $(CFLAGS) src/day$(day).c src/common.c src/common_c.c -o bin/day$(day)
	