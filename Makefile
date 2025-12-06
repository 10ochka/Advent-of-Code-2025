CC		= clang
CFLAGS	= -Wall -Wextra -Wpedantic -std=c23 -O3
STRINGS = ../vnls/vnls.c



day1-1: day1/part1.c
	@ cd day1 && $(CC) $(CFLAGS) $(STRINGS) part1.c -o part1
	@ cd day1 && ./part1

day1-2: day1/part2.c
	@ cd day1 && $(CC) $(CFLAGS) $(STRINGS) part2.c -o part2
	@ cd day1 && ./part2

day2-1: day2/part1.c
	@ cd day2 && $(CC) $(CFLAGS) $(STRINGS) part1.c -o part1
	@ cd day2 && ./part1

day2-2: day2/part2.c
	@ cd day2 && $(CC) $(CFLAGS) $(STRINGS) part2.c -o part2
	@ cd day2 && ./part2

day3-1: day3/part1.c
	@ cd day3 && $(CC) $(CFLAGS) $(STRINGS) part1.c -o part1
	@ cd day3 && ./part1

day3-2: day3/part2.c
	@ cd day3 && $(CC) $(CFLAGS) $(STRINGS) part2.c -o part2
	@ cd day3 && ./part2

day4-1: day4/part1.c
	@ cd day4 && $(CC) $(CFLAGS) $(STRINGS) part1.c -o part1
	@ cd day4 && ./part1

day4-2: day4/part2.c
	@ cd day4 && $(CC) $(CFLAGS) $(STRINGS) part2.c -o part2
	@ cd day4 && ./part2

day5-1: day5/part1.c
	@ cd day5 && $(CC) $(CFLAGS) $(STRINGS) part1.c -o part1
	@ cd day5 && ./part1

day5-2: day5/part2.c
	@ cd day5 && $(CC) $(CFLAGS) $(STRINGS) part2.c -o part2
	@ cd day5 && ./part2

day6-1: day6/part1.c
	@ cd day6 && $(CC) $(CFLAGS) $(STRINGS) part1.c -o part1
	@ cd day6 && ./part1

day6-2: day6/part2.c
	@ cd day6 && $(CC) $(CFLAGS) $(STRINGS) part2.c -o part2
	@ cd day6 && ./part2

.PHONY: day1-1 day1-2 \
		day2-1 day2-2 \
		day3-1 day3-2 \
		day4-1 day4-2 \
		day5-1 day5-2 \
		day6-1 day6-2
