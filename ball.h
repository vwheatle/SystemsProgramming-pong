#pragma once

#include <stdbool.h>

#include <curses.h>

#include "geometry.h"

// game settings

#define BLANK      ' '
#define DFL_SYMBOL 'o'

#define TOP_ROW    5
#define BOT_ROW    20
#define LEFT_EDGE  10
#define RIGHT_EDGE 70

#define X_INIT 10 // starting column
#define Y_INIT 10 // starting row

#define TICKS_PER_SEC 50 // affects speed

#define X_TTM 5
#define Y_TTM 8

struct ball_obj {
	vec2i pos, dir;
	vec2i old_pos;

	vec2i ticks_total;
	vec2i ticks_left;

	char symbol;
	bool redraw;
};

void ball_setup(struct ball_obj *);
void ball_update(struct ball_obj *);
bool ball_draw(struct ball_obj *);

bool bounce_or_lose(struct ball_obj *);

void ball_setup(struct ball_obj *ball) {
	ball->pos = (vec2i) {X_INIT, Y_INIT};
	ball->dir = (vec2i) {1, 1};

	ball->old_pos = ball->pos;

	ball->ticks_left = ball->ticks_total = (vec2i) {X_TTM, Y_TTM};

	ball->symbol = DFL_SYMBOL;

	ball->redraw = true;
}

void ball_update(struct ball_obj *ball) {
	vec2i old_pos = ball->pos;

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

	if (ball->redraw) {
		mvaddch(ball->pos.y, ball->pos.x, BLANK);
		bounce_or_lose(ball);
	}
}

bool ball_draw(struct ball_obj *ball) {
	bool drawn;
	if (drawn = ball->redraw) {
		mvaddch(ball->pos.y, ball->pos.x, ball->symbol);
		ball->redraw = false;
	}
	return drawn;
}

bool bounce_or_lose(struct ball_obj *ball) {
	bool r = false;

	if (ball->pos.y == TOP_ROW) {
		ball->dir.y = 1;
		r = true;
	} else if (ball->pos.y == BOT_ROW) {
		ball->dir.y = -1;
		r = true;
	}
	if (ball->pos.x == LEFT_EDGE) {
		ball->dir.x = 1;
		r = true;
	} else if (ball->pos.x == RIGHT_EDGE) {
		ball->dir.x = -1;
		r = true;
	}

	return r;
}
