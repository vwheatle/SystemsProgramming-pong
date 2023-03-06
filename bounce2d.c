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

#include "bounce.h"

struct {
	WINDOW *window;
	struct ball_obj ball;
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
		if (c == 'f')
			game.ball.ticks_total.x--;
		else if (c == 's')
			game.ball.ticks_total.x++;
		else if (c == 'F')
			game.ball.ticks_total.y--;
		else if (c == 'S')
			game.ball.ticks_total.y++;
	}

	wrap_up();
}

/*
 *	init structure and other stuff
 */
void set_up() {
	ball_setup(&game.ball);
	game.window = initscr();
	noecho();
	crmode();

	signal(SIGINT, SIG_IGN);
	mvaddch(game.ball.pos.y, game.ball.pos.x, game.ball.symbol);
	refresh();

	signal(SIGALRM, update);
	set_ticker(1000 / TICKS_PER_SEC); /* send millisecs per tick */
}

void update(int signum) {
	// disarm alarm
	signal(SIGALRM, SIG_IGN); // don't get caught now

	ball_update(&game.ball);

	ball_draw(&game.ball);

	// arm alarm
	signal(SIGALRM, update);
}

void wrap_up() {
	set_ticker(0);
	endwin(); /* put back to normal	*/
}
