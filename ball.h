#pragma once

#include <stdbool.h>

#include "geometry.h"
#include "wall.h"

typedef struct {
	vec2i pos, dir;

	vec2i ticks_total;
	vec2i ticks_left;

	char symbol;

	wall_obj *walls;
	size_t walls_len;

	vec2i draw_pos;
	bool redraw;
} ball_obj;

void ball_setup(ball_obj *);
void ball_update(ball_obj *);
bool ball_draw(ball_obj *);
