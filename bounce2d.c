/*  bounce2d 1.0
 *	bounce a character (default is 'o') around the screen
 *	defined by some parameters
 *
 *	user input: 	s slow down x component, S: slow y component
 *		 	f speed up x component,  F: speed y component
 *			Q quit
 *
 *	blocks on read, but timer tick sends SIGALRM caught by ball_update
 */

#include <curses.h>
#include <signal.h>

#include "ball.h"

#define sizeofarr(arr) (sizeof(arr) / sizeof(*arr))

struct {
	WINDOW *window;
	struct ball_obj ball[16];
} game;

/**  the main loop  **/

void set_up();
// void tick();
void update(int signum);
void wrap_up();

extern int set_ticker(int);

int main() {
	set_up();

	int c;
	while ((c = getchar()) != 'Q') {
		for (size_t i = 0; i < sizeofarr(game.ball); i++) {
			if (c == 'f')
				game.ball[i].ticks_total.x--;
			else if (c == 's')
				game.ball[i].ticks_total.x++;
			else if (c == 'F')
				game.ball[i].ticks_total.y--;
			else if (c == 'S')
				game.ball[i].ticks_total.y++;
		}
	}

	wrap_up();
}

/*
 *	init structure and other stuff
 */
void set_up() {
	for (size_t i = 0; i < sizeofarr(game.ball); i++) {
		ball_setup(&game.ball[i]);
		game.ball[i].pos.y += (game.ball[i].pos.x + i) / 16;
		game.ball[i].pos.x += i % 16;
		if (i & 1) game.ball[i].dir.x = -1;
		if (i & 2) game.ball[i].dir.y = -1;
	}
	game.window = initscr();
	noecho();
	crmode();

	signal(SIGINT, SIG_IGN);
	// TODO: call draw functions here

	signal(SIGALRM, update);
	set_ticker(1000 / TICKS_PER_SEC); /* send millisecs per tick */
}

void update(int signum) {
	// disarm alarm
	signal(SIGALRM, SIG_IGN); // don't get caught now

	/*** update functions zone ***/

	for (size_t i = 0; i < sizeofarr(game.ball); i++) {
		ball_update(&game.ball[i]);
	}

	/*** draw functions zone ***/

	bool drawn = false;
	for (size_t i = 0; i < sizeofarr(game.ball); i++) {
		drawn |= ball_draw(&game.ball[i]);
	}
	if (drawn) {
		move(LINES - 1, COLS - 1);
		refresh();
	}

	// arm alarm
	signal(SIGALRM, update);
}

void wrap_up() {
	set_ticker(0);
	endwin(); /* put back to normal	*/
}
