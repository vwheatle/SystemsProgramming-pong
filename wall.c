#include <stdbool.h>

#include <curses.h>

#include "geometry.h"
#include "wall.h"

// questionable function (but it works.)
void draw_rect(rect2i rect, bool visible) {
	vec2i tl = rect.pos;
	vec2i br = rect_bottom_right(rect);

	if (tl.x > br.x || tl.y > br.y) return;
	if (!rect.size.width || !rect.size.height) return;

	if (rect.size.width == 1 && rect.size.height == 1) {
		mvaddch(tl.y, tl.x, visible ? '+' : ' ');
		return;
	}

	if (rect.size.width == 1) tl.y--, br.y++;
	if (rect.size.height == 1) tl.x--, br.x++;

	if (rect.size.width > 1 && rect.size.height > 1) {
		mvaddch(tl.y, tl.x, visible ? ',' : ' ');
		mvaddch(tl.y, br.x, visible ? ',' : ' ');
		mvaddch(br.y, tl.x, visible ? '\'' : ' ');
		mvaddch(br.y, br.x, visible ? '\'' : ' ');
	}

	for (int x = tl.x + 1; x < br.x; x++) {
		mvaddch(tl.y, x, visible ? '-' : ' ');
		if (tl.y != br.y) mvaddch(br.y, x, visible ? '-' : ' ');
	}
	for (int y = tl.y + 1; y < br.y; y++) {
		mvaddch(y, tl.x, visible ? '|' : ' ');
		if (tl.x != br.x) mvaddch(y, br.x, visible ? '|' : ' ');
	}
}

void wall_setup(struct wall_obj *wall) {
	wall->draw_rect = wall->rect;
	wall->redraw = true;
}

void wall_update(struct wall_obj *wall) {
	if (!rect2i_eq(wall->rect, wall->draw_rect)) {
		wall->redraw = true;
		draw_rect(wall->draw_rect, false);
	}
}

bool wall_draw(struct wall_obj *wall) {
	bool drawn;
	if ((drawn = wall->redraw)) {
		draw_rect(wall->rect, true);

		wall->draw_rect = wall->rect;
		wall->redraw = false;
	}
	return drawn;
}
