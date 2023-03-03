/*  bounce2d 1.0
 *	bounce a character (default is 'o') around the screen
 *	defined by some parameters
 *
 *	user input: 	s slow down x component, S: slow y component
 *		 	f speed up x component,  F: speed y component
 *			Q quit
 *
 *	blocks on read, but timer tick sends SIGALRM caught by ball_move
 */

#include <curses.h>
#include <signal.h>

#include "bounce.h"

struct {
	WINDOW *window;
	struct ppball ball;
} game;

/**  the main loop  **/

void set_up();
void update();
void wrap_up();

void ball_move(struct ppball *);
bool bounce_or_lose(struct ppball *);

extern int set_ticker(int);

int main() {
	set_up();

	int c;
	while ((c = getchar()) != 'Q') {
		if (c == 'f')
			game.ball.ttm.x--;
		else if (c == 's')
			game.ball.ttm.x++;
		else if (c == 'F')
			game.ball.ttm.y--;
		else if (c == 'S')
			game.ball.ttm.y++;
	}

	wrap_up();
}

/*
 *	init structure and other stuff
 */
void set_up() {
	game.ball.pos.y = Y_INIT;
	game.ball.pos.x = X_INIT;
	game.ball.ttg.y = game.ball.ttm.y = Y_TTM;
	game.ball.ttg.x = game.ball.ttm.x = X_TTM;
	game.ball.dir.y = 1;
	game.ball.dir.x = 1;
	game.ball.symbol = DFL_SYMBOL;

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

	ball_move(&game.ball);

	// arm alarm
	signal(SIGALRM, update);
}

void wrap_up() {
	set_ticker(0);
	endwin(); /* put back to normal	*/
}

void ball_move(struct ppball *ball) {
	// old position
	vec2i old;
	old.y = ball->pos.y;
	old.x = ball->pos.x;

	// if it moved
	bool moved = false;

	if (ball->ttm.y > 0 && ball->ttg.y-- == 1) {
		ball->pos.y += ball->dir.y; // move y
		ball->ttg.y = ball->ttm.y;  // reset timer
		moved = true;
	}

	if (ball->ttm.x > 0 && ball->ttg.x-- == 1) {
		ball->pos.x += ball->dir.x; // move x
		ball->ttg.x = ball->ttm.x;  // reset timer
		moved = true;
	}

	if (moved) {
		// erase old character
		mvaddch(old.y, old.x, BLANK);
		// draw new character
		mvaddch(ball->pos.y, ball->pos.x, ball->symbol);

		bounce_or_lose(ball);

		move(LINES - 1, COLS - 1);
		refresh();
	}
}

bool bounce_or_lose(struct ppball *ball) {
	bool r = false;

	if (game.ball.pos.y == TOP_ROW) {
		game.ball.dir.y = 1;
		r = true;
	} else if (game.ball.pos.y == BOT_ROW) {
		game.ball.dir.y = -1;
		r = true;
	}
	if (game.ball.pos.x == LEFT_EDGE) {
		game.ball.dir.x = 1;
		r = true;
	} else if (game.ball.pos.x == RIGHT_EDGE) {
		game.ball.dir.x = -1;
		r = true;
	}

	return r;
}
