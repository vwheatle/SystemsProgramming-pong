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

#include "gameobject.h"
#include "geometry.h"

#include "ball.h"

#define sizeofarr(arr) (sizeof(arr) / sizeof(*arr))

static struct {
	struct ball_obj ball_self[16];
	game_obj game_objects[16];
} game;

void set_up();
void wrap_up();
void update(int signum);

extern int set_ticker(int);

int main() {
	set_up();

	int c;
	while ((c = getchar()) != 'Q') {
		for (size_t i = 0; i < sizeofarr(game.ball_self); i++) {
			if (c == 'f')
				game.ball_self[i].ticks_total.x--;
			else if (c == 's')
				game.ball_self[i].ticks_total.x++;
			else if (c == 'F')
				game.ball_self[i].ticks_total.y--;
			else if (c == 'S')
				game.ball_self[i].ticks_total.y++;
		}
	}

	wrap_up();
}

/*
 *	init structure and other stuff
 */
void set_up() {
	for (size_t i = 0; i < sizeofarr(game.ball_self); i++) {
		game.game_objects[i].vtable = ball_obj_vtable;
		game.game_objects[i].self = &game.ball_self[i];

		ball_setup(&game.ball_self[i]);
		game.ball_self[i].pos.y += (game.ball_self[i].pos.x + i) / 16;
		game.ball_self[i].pos.x += i % 16;
		if (i & 1) game.ball_self[i].dir.x = -1;
		if (i & 2) game.ball_self[i].dir.y = -1;
		game.ball_self[i].ticks_total.x += i >> 2;
		game.ball_self[i].ticks_total.y += i >> 1;
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

void update(int signum) {
	// don't want to risk signal calling update inside of previous update call
	signal(SIGALRM, SIG_IGN); // disarm alarm

	/*** update functions zone ***/

	for (size_t i = 0; i < sizeofarr(game.game_objects); i++) {
		game.game_objects[i].vtable.update(game.game_objects[i].self);
		game.game_objects[i].vtable.undraw(game.game_objects[i].self);
	}

	/*** draw functions zone ***/

	bool drawn = false;
	for (size_t i = 0; i < sizeofarr(game.game_objects); i++) {
		drawn |= game.game_objects[i].vtable.draw(game.game_objects[i].self);
	}
	if (drawn) {
		// move the cursor to the bottom right of the window
		move(LINES - 1, COLS - 1);

		// flush all changes of the window to the terminal
		refresh();
	}

	signal(SIGALRM, update); // arm alarm
}
