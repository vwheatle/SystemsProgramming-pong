#pragma once

#include <stdbool.h>

#include <curses.h>

#include "geometry.h"
#include "gameobject.h"

struct ball_obj {
	vec2i pos, dir;

	vec2i ticks_total;
	vec2i ticks_left;

	char symbol;

	vec2i draw_pos;
	bool redraw;
};

void ball_setup(struct ball_obj *);
void ball_update(struct ball_obj *);
bool ball_draw(struct ball_obj *);
void ball_undraw(struct ball_obj *);

bool bounce_or_lose(struct ball_obj *);

static game_obj_vtable ball_obj_vtable = {
	(game_obj_update_fn *)(&ball_update),
	(game_obj_draw_fn *)(&ball_draw),
	(game_obj_undraw_fn *)(&ball_undraw),
};
