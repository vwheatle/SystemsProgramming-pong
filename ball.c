#include <stdbool.h>

#include <curses.h>

#include "geometry.h"
#include "ball.h"

// game settings

#define X_INIT 3 // starting column
#define Y_INIT 2 // starting row

#define X_TTM 5
#define Y_TTM 8

// implementations

void ball_setup(struct ball_obj *ball) {
	ball->pos = (vec2i) {X_INIT, Y_INIT};
	ball->dir = (vec2i) {1, 1};

	ball->ticks_left = ball->ticks_total = (vec2i) {X_TTM, Y_TTM};

	ball->symbol = 'o';

	ball->draw_pos = ball->pos;
	ball->redraw = true;
}

void ball_update(struct ball_obj *ball) {
	if (ball->ticks_total.y > 0 && ball->ticks_left.y-- == 1) {
		ball->pos.y += ball->dir.y;               // move y
		ball->ticks_left.y = ball->ticks_total.y; // reset timer
		ball->redraw = true;                      // need to redraw!
	}

	if (ball->ticks_total.x > 0 && ball->ticks_left.x-- == 1) {
		ball->pos.x += ball->dir.x;               // move x
		ball->ticks_left.x = ball->ticks_total.x; // reset timer
		ball->redraw = true;                      // need to redraw!
	}

	if (ball->redraw) bounce_or_lose(ball);
}

bool ball_draw(struct ball_obj *ball) {
	bool drawn;
	if ((drawn = ball->redraw)) {
		mvaddch(ball->pos.y, ball->pos.x, ball->symbol);
		ball->draw_pos = ball->pos;
		ball->redraw = false;
	}
	return drawn;
}

void ball_undraw(struct ball_obj *ball) {
	if (ball->redraw) mvaddch(ball->draw_pos.y, ball->draw_pos.x, ' ');
}

bool bounce_or_lose(struct ball_obj *ball) {
	bool r = false;

	if (ball->pos.y <= TOP_ROW) {
		ball->dir.y = 1;
		r = true;
	} else if (ball->pos.y >= BOT_ROW) {
		ball->dir.y = -1;
		r = true;
	}
	if (ball->pos.x <= LEFT_EDGE) {
		ball->dir.x = 1;
		r = true;
	} else if (ball->pos.x >= RIGHT_EDGE) {
		ball->dir.x = -1;
		r = true;
	}

	return r;
}
