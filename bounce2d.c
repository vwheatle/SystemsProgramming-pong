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

#include "set_ticker.h"

#include "geometry.h"

#include "wall.h"
#include "ball.h"

#define sizeofarr(arr) (sizeof(arr) / sizeof(*arr))

static struct {
	struct ball_obj ball[16];
	struct wall_obj wall[4];
} game;

void set_up();
void wrap_up();
void update(int signum);

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
		if (c == 'j')
			game.wall[0].rect.pos.y++, game.wall[1].rect.pos.y++;
		else if (c == 'k')
			game.wall[0].rect.pos.y--, game.wall[1].rect.pos.y--;
	}

	wrap_up();
}

/*
 *	init structure and other stuff
 */
void set_up() {
	for (size_t i = 0; i < sizeofarr(game.wall); i++) {
		game.wall[i].rect = (rect2i) {{(i + 1) * 5, 5}, {i < 2 ? 1 : 5, 5}};
		wall_setup(&game.wall[i]);
	}
	for (size_t i = 0; i < sizeofarr(game.ball); i++) {
		ball_setup(&game.ball[i]);

		game.ball[i].pos.y += (game.ball[i].pos.x + i) / 16;
		game.ball[i].pos.x += i % 16;

		if (i & 1) game.ball[i].dir.x = -1;
		if (i & 2) game.ball[i].dir.y = -1;

		game.ball[i].ticks_total.x += i >> 2;
		game.ball[i].ticks_total.y += i >> 1;

		game.ball[i].walls = game.wall;
		game.ball[i].walls_len = sizeofarr(game.wall);
	}

	initscr(); // give me a new screen buffer (a "window")
	noecho();  // don't echo characters as i type them
	crmode();  // don't process line breaks or delete characters

	signal(SIGINT, SIG_IGN);          // ignore interrupt signals (Ctrl+C)
	set_ticker(1000 / TICKS_PER_SEC); // param is in millisecs per tick

	update(SIGALRM); // tail call into update (and pretend the call
	                 //  was from the ticker triggering a SIGALRM)
}

void wrap_up() {
	set_ticker(0); // disable sending of SIGALRM at constant interval
	endwin();      // destroy my window
}

void update(__attribute__((unused)) int signum) {
	// don't want to risk signal calling update inside of previous update call
	signal(SIGALRM, SIG_IGN); // disarm alarm

	/*** update functions zone ***/

	for (size_t i = 0; i < sizeofarr(game.wall); i++) {
		wall_update(&game.wall[i]);
	}
	for (size_t i = 0; i < sizeofarr(game.ball); i++) {
		ball_update(&game.ball[i]);
	}

	/*** draw functions zone ***/

	bool drawn = false;
	for (size_t i = 0; i < sizeofarr(game.wall); i++) {
		drawn |= wall_draw(&game.wall[i]);
	}
	for (size_t i = 0; i < sizeofarr(game.ball); i++) {
		drawn |= ball_draw(&game.ball[i]);
	}

	if (drawn) {
		// move the cursor to the bottom right of the window
		move(LINES - 1, COLS - 1);

		// flush all changes of the window to the terminal
		refresh();
	}

	signal(SIGALRM, &update); // arm alarm
}
