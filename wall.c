#include "wall.h"

void wall_setup(struct wall_obj *wall) {
	wall->draw_pos = wall->rect.pos;
	wall->redraw = true;
}

void wall_update(struct wall_obj *wall) {}
bool wall_draw(struct wall_obj *wall) { return false; }
