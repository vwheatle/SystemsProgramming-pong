#pragma once

#include <stdbool.h>

#include "geometry.h"
#include "wall.h"

struct ball_obj {
	vec2i pos, dir;

	vec2i ticks_total;
	vec2i ticks_left;

	char symbol;

	struct wall_obj *walls;
	size_t walls_len;

	vec2i draw_pos;
	bool redraw;
};

void ball_setup(struct ball_obj *);
void ball_update(struct ball_obj *);
bool ball_draw(struct ball_obj *);

bool bounce_or_lose(struct ball_obj *);
