#
# makefile for pong_game - like pong, including walls (paddle)
#

CC = gcc
CC_FLAGS = -std=c99 -Wall -Wpedantic -Wextra -fsanitize=undefined
VALGRIND_FLAGS = --quiet --tool=memcheck --leak-check=yes --show-reachable=yes --num-callers=3 --error-exitcode=1

ALL_OBJECTS = game.o ball.o wall.o geometry.o

all: main

geometry.o: geometry.c geometry.h
	$(CC) $(CC_FLAGS) -c -o geometry.o geometry.c
# (despite using ncurses in preprocessor stuff, ncurses isn't actually used
#  in the body of its functions. those are just simple utility functions.)

wall.o: wall.c wall.h
	$(CC) $(CC_FLAGS) -c -o wall.o wall.c -lncurses

ball.o: ball.c ball.h
	$(CC) $(CC_FLAGS) -c -o ball.o ball.c -lncurses

game.o: game.c game.h
	$(CC) $(CC_FLAGS) -c -o game.o game.c -lncurses

main: $(ALL_OBJECTS) main.c set_ticker.h
	$(CC) $(CC_FLAGS) -o pong_game main.c $(ALL_OBJECTS) -lncurses

# -c for compiling but not linking
# -g for debugging with gdb...

clean:
	rm -f *.o
	rm -f ./pong_game
