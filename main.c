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

#include "set_ticker.h" // -> set_ticker()

#include "game.h" // -> game_obj

void set_up();
void update(int signum);
void wrap_up();

static game_obj game;

int main() {
	set_up();

	int key;
	while ((key = getchar()) != 'Q') game_input(&game, key);

	wrap_up();
}

void set_up() {
	initscr(); // give me a new screen buffer (a "window")
	noecho();  // don't echo characters as i type them
	crmode();  // don't process line breaks or delete characters

	game_setup(&game); // set up game state

	signal(SIGINT, SIG_IGN);          // ignore interrupt signals (Ctrl+C)
	set_ticker(1000 / TICKS_PER_SEC); // param is in millisecs per tick

	update(SIGALRM); // tail call into update (and pretend the call
	                 //  was from the ticker triggering a SIGALRM)
}

void update(__attribute__((unused)) int signum) {
	// don't want to risk signal calling update inside of previous update call
	signal(SIGALRM, SIG_IGN); // disarm alarm

	game_update(&game);

	// if the game actually changed anything
	// about the screen, update the screen.
	if (game_draw(&game)) {
		// move the cursor to the bottom right of the window
		move(LINES - 1, COLS - 1);

		// flush all changes of the window to the terminal
		refresh();
	}

	signal(SIGALRM, &update); // arm alarm
}

void wrap_up() {
	set_ticker(0); // disable sending of SIGALRM at constant interval
	endwin();      // destroy my window
}
