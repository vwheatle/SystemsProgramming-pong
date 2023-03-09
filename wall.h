#pragma once

#include "geometry.h"

struct wall_obj {
	rect2i rect;

	rect2i draw_rect;
	bool redraw;
};

// ouugh walls are just paddles that don't move

void draw_rect(rect2i rect, bool visible);

void wall_setup(struct wall_obj *);
void wall_update(struct wall_obj *);
bool wall_draw(struct wall_obj *);
