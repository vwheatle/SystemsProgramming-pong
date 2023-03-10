#
# makefile for bounce2d - sort of like pong, but with no walls or paddle
#

CC = gcc
CC_FLAGS = -std=c99 -Wall -Wpedantic -Wextra -fsanitize=undefined
VALGRIND_FLAGS = --quiet --tool=memcheck --leak-check=yes --show-reachable=yes --num-callers=3 --error-exitcode=1

all: bounce2d

geometry.o: geometry.c geometry.h
	$(CC) $(CC_FLAGS) -c -o geometry.o geometry.c

ball.o: ball.c ball.h
	$(CC) $(CC_FLAGS) -c -o ball.o ball.c -lncurses

wall.o: wall.c wall.h
	$(CC) $(CC_FLAGS) -c -o wall.o wall.c -lncurses

game.o: ball.o wall.o geometry.o
	$(CC) $(CC_FLAGS) -c -o game.o game.c -lncurses

bounce2d: game.o bounce2d.c set_ticker.h
	$(CC) $(CC_FLAGS) -o bounce2d bounce2d.c game.o ball.o wall.o geometry.o -lncurses

# -c for compiling but not linking
# -g for debugging with gdb...

clean:
	rm -f *.o
	rm -f ./bounce2d
