

CC = clang
CFLAGS = -Wall -Wextra -Wpedantic -std=c23 -O3
STRINGS = ../vnls/vnls.c



day1-1: day1/part1.c
	@ cd day1 && $(CC) $(CFLAGS) $(STRINGS) part1.c -o part1
	@ cd day1 && ./part1


day1-2: day1/part2.c
	@ cd day1 && $(CC) $(CFLAGS) $(STRINGS) part2.c -o part2
	@ cd day1 && ./part2


.PHONY: day1-1 day1-2
