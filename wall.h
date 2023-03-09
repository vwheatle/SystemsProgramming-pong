#pragma once

#include "geometry.h"

#define PADDLE_HEIGHT   5
#define PADDLE_OFFSET_X 3
#define PADDLE_START_Y  ((LINES - PADDLE_HEIGHT) / 2)
#define PADDLE_SIZE \
	(size2i) { 1, PADDLE_HEIGHT }

struct wall_obj {
	rect2i rect;

	rect2i draw_rect;
	bool redraw;
};

// ouugh walls are just paddles that don't move

void wall_setup(struct wall_obj *);
void wall_update(struct wall_obj *);
bool wall_draw(struct wall_obj *);
