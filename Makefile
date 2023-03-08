#
# makefile for bounce2d - sort of like pong, but with no walls or paddle
#

CC = gcc
CC_FLAGS = -std=c99 -Wall -Wpedantic -Wextra -fsanitize=undefined
VALGRIND_FLAGS = --quiet --tool=memcheck --leak-check=yes --show-reachable=yes --num-callers=3 --error-exitcode=1

all: bounce2d

ball.o: ball.c ball.h geometry.h
	$(CC) $(CC_FLAGS) -c -o ball.o ball.c -lncurses
# -c for compiling but not linking

bounce2d: ball.o bounce2d.c set_ticker.h
	$(CC) $(CC_FLAGS) -o bounce2d bounce2d.c ball.o -lncurses
# -g for debugging with gdb...

clean:
	rm -f *.o
	rm -f ./bounce2d
