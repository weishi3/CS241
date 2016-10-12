EXE = part1 part2
OBJS = functions1.o functions2.o

COMPILER = gcc
COMPILER_OPTS = -c -g -O0 -Wall -Werror -std=c99
LINKER = gcc
LINKER_OPTS =

all: $(EXE) 

part1: part1-main.c functions1.o
	$(LINKER) part1-main.c functions1.o -o part1
part2: part2-main.c functions2.o
	$(LINKER) part2-main.c functions2.o -o part2


functions1.o: part1-functions.c part1-functions.h
	$(COMPILER) $(COMPILER_OPTS) part1-functions.c -o functions1.o

functions2.o: part2-functions.c part2-functions.h
	$(COMPILER) $(COMPILER_OPTS) part2-functions.c -o functions2.o

clean:
	rm -f part1 part2 *.o
