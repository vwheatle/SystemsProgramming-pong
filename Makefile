#
# makefile for bounce2d - sort of like pong, but with no walls or paddle
#

CC = gcc
CC_FLAGS = -std=c99 -Wall -Wpedantic -Wextra -fsanitize=undefined
VALGRIND_FLAGS = --quiet --tool=memcheck --leak-check=yes --show-reachable=yes --num-callers=3 --error-exitcode=1

all: bounce2d

bounce2d: bounce2d.o set_ticker.o ball.h geometry.h
	$(CC) $(CC_FLAGS) -o bounce2d bounce2d.o set_ticker.o -lncurses
# -g for debugging with gdb...

clean:
	rm -f *.o
