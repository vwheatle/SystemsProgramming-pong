#pragma once

#include "geometry.h"

struct wall_obj {
	rect2i rect;

	vec2i draw_pos;
	bool redraw;
};

// ouugh walls are just paddles that don't move

void wall_setup(struct wall_obj *);
void wall_update(struct wall_obj *);
bool wall_draw(struct wall_obj *);
